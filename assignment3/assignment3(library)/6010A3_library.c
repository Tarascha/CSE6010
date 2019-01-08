//
//  6010A3_library.c
//  6010A3
//
//  Created by 刘昊 on 10/9/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#include "6010A3_library.h"
//timeline
double Now = 0.0;
//statistics for calculation;
int enteredcustomer = 0;//number of customers entered this system
int exitedcustomer = 0;//number fo customers exited this system
//The minimum, maximum, and average amount of time a customer remains in the system among those customers who exited during the simulation run.---------------
double minimumtime = 1000.0;//update in arrival events in exit;
double maximumtime = 0.0;
double sumtime = 0.0;
//-----------------------
//The minimum, maximum, and average amount of the total time each customer must wait in queues in traveling through the system.--------------
double minimumtimeinQS = 1000.0;// update in arrival events in exit;
double maximumtimeinQS = 0.0;
double sumtimeinQS = 0.0;
//-------------------------------------------
////simulation engine...
void EventHandler (void *data);

typedef struct event{
    double timestamp;
    void* AppData;
    struct event* next;
}Event;

Event FEL = {-1, 0 ,0};

void Schedule(double ts, void* data){
    Event *e, *p, *q;
    
    // create event data structure and fill it in
    if ((e = malloc (sizeof (Event))) == NULL) exit(1);
    e->timestamp = ts;
    e->AppData = data;
    
    // insert into priority queue
    // p is lead pointer, q is trailer
    for (q=&FEL, p=FEL.next; p!=NULL; p=p->next, q=q->next) {
        if (p->timestamp >= e->timestamp) break;
    }
    // insert after q (before p)
    e->next = q->next;
    q->next = e;
}

double CurrentTime (void)
{
    return (Now);
}

 Event *Remove (void)
{
     Event *e;
    
    if (FEL.next==NULL) return (NULL);
    e = FEL.next;        // remove first event in list
    FEL.next = e->next;
    return (e);
}

void PrintList (void)
{
     Event *p;
    
    printf ("Event List: ");
    for (p=FEL.next; p!=NULL; p=p->next) {
        printf ("%f ", p->timestamp);
    }
    printf ("\n");
}

void RunSim (double EndTime)
{
     Event *e;
    
    printf ("Initial event list:\n");
    PrintList ();
    
    // Main scheduler loop
    while ((e=Remove()) != NULL) {
        Now = e->timestamp;
        if (Now > EndTime) break;
        EventHandler(e->AppData);
        free (e);    // it is up to the event handler to free memory for parameters
        PrintList ();
    }
}
// simulation enging is all above
//---------------------------------
// simulation application
// maximum number of components that can be defined for the simulation
#define MAXCOMPONENTS    100

// Component types
#define     GENERATOR       0
#define     FORK            1
#define     QUEUE_STATION   2
#define     EXIT            3
// Event types: Generate new customer, arrival at a component, departure from queue station
#define     GENERATE    0
#define     ARRIVAL     1
#define     DEPARTURE   2

struct EventData{
    int EventType;// Type of event (GENERATE, ARRIVAL, DEPARTURE)
    struct Customer *Cust;// Arriving or departing customer; unused for GENERATE events
    int CompID;//ID of component where customer created, arrives, or departs
};
// global, master list of components
struct {
    int ComponentType;  // GENERATOR, QUEUE_STATION, FORK, EXIT
    void *Comp;         // Pointer to information on component (Generator, Exit struct, etc.)
} Component[MAXCOMPONENTS];

struct Generator {//struct for component G
    double IntArrTime;     // mean interarrival time for generated components
    int DestComp;          // ID of next component customers are sent to
};

struct Exit {// struct for component E
    int Count;              // number of customers that exited at this component
};

struct Customer {
    double CreationTime;        // time customer was created
    double ExitTime;            // time customer departs system
    double EnterQSTime;         // time customer enter a QS
    double TotalWaitTimeinQS;   // total time customer wait in QS;
    struct Customer *next;      // pointer to next customer when it is queued in a FIFO
};

struct FIFOQueue {//struct for component QS
    struct Customer *first;     // pointer to first customer in queue
    struct Customer *last;      // pointer to last customer in queue
    double servetime;           // average serve time of a QS
    //int FromComp;  //seems unused             // ID of what component customers are sent from
    int DestComp;               // ID of next component customers are sent to
    double totalwaitingtimeofCust; // for static 4.
    int numberofServedCust;
};

struct Fork{//struct for component Fork
    //int FromComp;
    int NumberofOutputs;
    int* IDofOutputs;
    double* possiblityofOutputs;
};

//--------------------------random generator------------------------------
double urand(void){
    double possibility = rand()/(double)(RAND_MAX/1);
    possibility = rand()/(double)(RAND_MAX/1);
    if(possibility == 1){
        while(possibility == 1){
            possibility = rand()/(double)(RAND_MAX/1);
            possibility = rand()/(double)(RAND_MAX/1);
        }
    }
    return possibility;
}
double randexp(int u){
   
    return -u*(log(1 - urand()));
}


int possion(int P)
{
    int  k = 0;
    long double p = 1.0;
    long double l = exp(-P);
    //srand((unsigned)time(NULL));
    while(p>=l)
    {
        double u = urand();
        p *= u;
        k++;
    }
    return k-1;
}
//------------------------------------------------------------------------------

//-------------------------make components------------------------------------------------
// Create a generator component with identifier GenID and interarrival time IntTime
// that is connected to component DestID
void MakeGenerator (int GenID, double IntTime, int DestID)
{   struct Generator *p;
    struct EventData *d;
    
    // Add component to master list; Caller is responsible for handling set up errors
    Component[GenID].ComponentType = GENERATOR;
    printf ("Creating Generator Component, ID=%d, Interarrival time=%f, Destination=%d\n", GenID, IntTime, DestID);
    
    // Allocate space for component, fill in parameters
    if ((p=malloc (sizeof(struct Generator))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
    p->IntArrTime = IntTime;
    p->DestComp = DestID;
    Component[GenID].Comp = p;
    
    // schedule initial, first generator event for this component
    if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
    d->EventType = GENERATE;
    d->Cust = NULL;
    d->CompID = GenID;
    Schedule (IntTime, d);  // should use random draw here
}
// Creat an Queue Station Component with identifier QSID and average serve time SerTime
// that take customer from FromID and send customer to component DestID
void MakeQS(int QSID, double SerTime, int DestID){
    struct FIFOQueue *p;
    //Add component to master list;
    Component[QSID].ComponentType = QUEUE_STATION;
    printf("Creating Queue Station Component, ID=%d, Average serve time=%f, Destination=%d\n", QSID, SerTime, DestID);
    //Allocate space for component, fill in parameters;
    if((p=malloc(sizeof(struct FIFOQueue))) == NULL){
        fprintf(stderr, "malloc error\n"); exit(1);
    }
    p->first = NULL;
    p->last = NULL;
    p->servetime = SerTime;
    //p->FromComp = FromID;
    p->DestComp = DestID;
    p->numberofServedCust = 0;//for static 4.
    p->totalwaitingtimeofCust = 0.0;
    Component[QSID].Comp = p;
}
// Creat a Frok component with identifier FrokID and totalnumber of output components of this Fork,
// which take customers from FromID;
void MakeFork(int ForkID, int totalnumber, int ID[], double possibility[] ){
    //int** outputs is a 2-dimensional matrix, with K rows and 2 columns
    // array "ID"  stores the ID of output components
    // array "possibility" the possibility customer are sent to corresponding components
    // above two array must be corresponding!
    // the possiblity is in "double" type;
    struct Fork *p;
    //Add component to master list;
    Component[ForkID].ComponentType = FORK;
    printf("Creating Fork Component, ID=%d,number of outputs=%d",ForkID, totalnumber);
    //Allocate space for component, fill in parameters;
    if((p=malloc(sizeof(struct Fork))) == NULL){
        fprintf(stderr,"malloc error\n");exit(1);
    }
    p->IDofOutputs = ID; // point to an the array which stores information of outputs;
    p->possiblityofOutputs =possibility;
    //p->FromComp = FromID;
    p->NumberofOutputs = totalnumber;
    Component[ForkID].Comp = p;
    printf("FORK EXITS:");
    for(int i=0; i< 6; i++){
        printf("%d ", p->IDofOutputs[i]);
    }
    printf("\n\n");
}
// Create an Exit Component with identifier ExitID
void MakeExit (int ExitID)
{   struct Exit *p;
    
    printf ("Creating Exit Component, ID=%d\n", ExitID);
    Component[ExitID].ComponentType = EXIT;
    
    // Allocate space for component, fill in parameters
    if ((p=malloc (sizeof(struct Exit))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
    p->Count = 0;
    Component[ExitID].Comp = p;
}
//-------------event functions-------------------------------------------------------------
// General Event Handler Procedure defined in simulation engine interface called by simulation engine
void Generate (struct EventData *e);
void Arrival (struct EventData *e);
void Departure(struct EventData*e);

void EventHandler (void *data)
{
    struct EventData *d;
    
    // coerce type so the compiler knows the type of information pointed to by the parameter data.
    d = (struct EventData *) data;
    // call an event handler based on the type of event
    if (d->EventType == GENERATE) Generate (d);
    else if (d->EventType == ARRIVAL) Arrival (d);
    else if (d->EventType == DEPARTURE) Departure(d);
    else {fprintf (stderr, "Illegal event found\n"); exit(1); }
    free (d); // Release memory for event paramters
}
// event handler for generate events

void Generate (struct EventData *e)
{
    struct EventData *d;
    struct Customer *NewCust;
    double ts;
    struct Generator *pGen;  // pointer to info on generator component
    
    if (e->EventType != GENERATE) {fprintf (stderr, "Unexpected event type\n"); exit(1);}
    printf ("Processing Generate event at time %f, Component=%d\n", CurrentTime(), e->CompID);
    
    if (Component[e->CompID].ComponentType != GENERATOR) {fprintf(stderr, "bad componenet type\n"); exit(1);}
    // Get pointer to state information for generator component
    pGen = (struct Generator *) Component[e->CompID].Comp;
    
    // Create a new customer
    if ((NewCust=malloc (sizeof(struct Customer))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
    enteredcustomer ++;
    NewCust->CreationTime = CurrentTime();
    NewCust->ExitTime = 0.0;    // undefined initially
    NewCust->EnterQSTime = 0.0; // undefined initially
    NewCust->TotalWaitTimeinQS = 0.0; // undefined initially
    NewCust->next = NULL;       // undefined initially
    
    // Schedule arrival event at component connected to generator
    if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
    d->EventType = ARRIVAL;
    d->Cust = NewCust;
    d->CompID = pGen->DestComp;
    ts = CurrentTime();
    Schedule (ts, d);
    
    // Schedule next generation event
    if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
    d->EventType = GENERATE;
    d->CompID = e->CompID;
   
    double temptime = possion(pGen->IntArrTime);
    ts = CurrentTime() + temptime;       // need to modify to exponential random number
    printf("\n\n\n randexp(intArr) = %lf\n\n\n",temptime);
    Schedule (ts, d);
}
// event handler for arrival events
void Arrival (struct EventData *e)
{
    struct EventData *d;
    double ts;
    //int Comp;                   // ID of component where arrival occurred
    struct Exit *pExit;         // pointer to info on exit component
    struct FIFOQueue *pFIFO;    //pointer to info on FIFOQUEUE component;
    struct Fork *pFork;         //pointer to info on Fork component;
    
    if (e->EventType != ARRIVAL) {fprintf (stderr, "Unexpected event type\n"); exit(1);}
    printf ("Processing Arrival Event\n");
    
    // processing depends on the type of component arrival occurred at
    if(Component[e->CompID].ComponentType == GENERATOR){
        printf("XXxxxxxxxxxxxxxxx\n");
        exit(1);
    }
    if (Component[e->CompID].ComponentType == EXIT) {
         printf("processing arrival event in EXIT,Compnent=%d, time=%f\n",e->CompID,CurrentTime());
        pExit = (struct Exit *) Component[e->CompID].Comp;
        (pExit->Count)++;       // number of exited customers
        exitedcustomer++;       // for static 1.
        // following for the state variables;
        e->Cust->ExitTime = CurrentTime(); // I don't know why a cust need a exitime? maybe for the avt time of each customer?//now I know lol
        //for static 2.
        double waittime = e->Cust->ExitTime - e->Cust->CreationTime;
        if(waittime > maximumtime){
            maximumtime = waittime;
        }
        if(waittime < minimumtime){
            minimumtime = waittime;
        }
        sumtime += waittime;
        //for static 3.
        double waittimeinQS = e->Cust->TotalWaitTimeinQS;
        if(waittimeinQS < minimumtimeinQS){
            minimumtimeinQS = waittimeinQS;
        }
        if(waittimeinQS > maximumtimeinQS){
            maximumtimeinQS = waittimeinQS;
        }
        sumtimeinQS += waittimeinQS;
        free (e->Cust);         // release memory of exiting customer
    }
    else if (Component[e->CompID].ComponentType == QUEUE_STATION) {
        // code for customer arrival at a queue station
        printf("processing arrival event in QS,Compnent=%d, time=%f\n",e->CompID,CurrentTime());
        pFIFO = (struct FIFOQueue*) Component[e->CompID].Comp;
        if(pFIFO->first == NULL && pFIFO->last == NULL){// if FIFO is empty, insert first Customer
            pFIFO->first = e->Cust;
            pFIFO->last = e->Cust;
            e->Cust->EnterQSTime = CurrentTime();// update the EnterQSTime for customer
            if((d=malloc(sizeof(struct EventData))) == NULL){
                fprintf(stderr, "malloc error\n"); exit(1);
            }
            d->EventType = DEPARTURE;// since FIFO is empty, QS directly serve this customer;
            d->Cust = e->Cust;
            d->CompID = e->CompID;
            
            double temptime = randexp(pFIFO->servetime);
            ts = CurrentTime() + temptime;
            printf("\n\n\n randexp(servetime) = %f\n\n\n", temptime);
            Schedule(ts, d);
        }else{// if FIFO is not empty, insert this Customer to the end of FIFO;
            pFIFO->last->next = e->Cust;// Customer has to wait in line;
            pFIFO->last = pFIFO->last->next;
        }
        
    }
    else if (Component[e->CompID].ComponentType == FORK) {
        // code for customer arrival at a fork component
        printf("processing arrival event in Fork,Component=%d, time=%f\n",e->CompID,CurrentTime());
        pFork = (struct Fork*) Component[e->CompID].Comp;
        int totaloutputs = pFork->NumberofOutputs;
        //srand((unsigned)time(NULL));// maybe it shoud be writen in main function
        
         double possibility = rand()/(double)(RAND_MAX/1);//
                possibility = rand()/(double)(RAND_MAX/1);//
        double stsum = 0; // sum of possibilitys, initialized with 0
        //sum of possibilitys, initialized with the first element in possibilty array;
        double edsum = pFork->possiblityofOutputs[0];
        double k[totaloutputs];// copy of the array of possiblities;
        for(int i = 0; i < totaloutputs; i++){
            k[i] = (pFork->possiblityofOutputs[i]);
        }
        for(int i = 0; i < totaloutputs; i++){
            if(possibility > stsum && possibility <= edsum){
                if((d=malloc(sizeof(struct EventData))) == NULL){
                    fprintf(stderr, "malloc error\n"); exit(1);
                }
                d->EventType = ARRIVAL;// once a customer arrives Fork, it directly be sent to output
                d->Cust = e->Cust;
                d->CompID = pFork->IDofOutputs[i]; // send customer to the output component;
                //---------------------
                printf("test/n");
                printf("FORK EXITS:");
                for(int i=0; i< 6; i++){
                    printf("%d ", pFork->IDofOutputs[i]);
                }
                printf("\n\n");
                printf("test\n");
                //------------------------
                ts = CurrentTime();
                Schedule(ts, d);
                break;
            }
            stsum += k[i];
            edsum += k[i + 1];// it won't get out of array's boundry, since this step will not excute
                              // in the last loop
        }
    }
    else {fprintf (stderr, "Bad component type for arrival event\n"); exit(1);}
}

void Departure(struct EventData*e){
    struct EventData *d;
    struct EventData *k; // for next departure in QueQue if QueQue is not empty;
    double ts;
    //int Comp;                   // ID of component where departure occurred
    struct FIFOQueue *pFIFO;    //pointer to info on FIFOQUEUE component;
    if (e->EventType != DEPARTURE) {fprintf (stderr, "Unexpected event type\n"); exit(1);}
    printf ("Processing departure event ,time %f, Component=%d\n", CurrentTime(), e->CompID);
    if(Component[e->CompID].ComponentType == QUEUE_STATION){
        printf("processing departure event in QS, component=%d, time=%f\n",e->CompID,CurrentTime());
    }
     pFIFO = (struct FIFOQueue*) Component[e->CompID].Comp;
    if(pFIFO->first == pFIFO->last){// if there is only one customer in FIFO
        if((d=malloc(sizeof(struct EventData))) == NULL){
            fprintf(stderr, "malloc error\n"); exit(1);
        }
        d->EventType = ARRIVAL;//generate following event
        d->Cust = pFIFO->first;
        d->CompID = pFIFO->DestComp;
       
//        //if a customer directly be served when entered the Queue, then there is no waitting time
//        double temptime = d->Cust->EnterQSTime + tempservetime;
        d->Cust->TotalWaitTimeinQS += (CurrentTime() - d->Cust->EnterQSTime);// update total waitQStime for Cust;
        
        //for static 4
        pFIFO->numberofServedCust ++;
        pFIFO->totalwaitingtimeofCust += CurrentTime() - d->Cust->EnterQSTime;
        //
        ts = CurrentTime();//schedule event;
        Schedule(ts, d);
        pFIFO->first = NULL;
        pFIFO->last = NULL;
    }else{// if there is more than on customer in FIFO
        if((d=malloc(sizeof(struct EventData))) == NULL){
            fprintf(stderr, "malloc error\n"); exit(1);
        }
        
        d->EventType = ARRIVAL;// schedule the customer to next station
        d->Cust = pFIFO->first;
        d->CompID = pFIFO->DestComp;
        
        //if a customer directly be served when entered the Queue, then there is no waitting time
        double tempservetime = randexp(pFIFO->servetime);
       
        d->Cust->TotalWaitTimeinQS += (CurrentTime() - d->Cust->EnterQSTime);// update total waitQStime for Cust;
        
        //for static 4
        pFIFO->numberofServedCust ++;
        pFIFO->totalwaitingtimeofCust += CurrentTime() - d->Cust->EnterQSTime;
        //
        ts = CurrentTime();
        Schedule(ts, d);
        if((k=malloc(sizeof(struct EventData))) == NULL){
            fprintf(stderr, "malloc error\n"); exit(1);
        }
        pFIFO->first = pFIFO->first->next;//point to next Customer,
        k->EventType = DEPARTURE;// shedule de departure of next customer in FIFO( e.g. serve next)
        k->Cust = pFIFO->first;
        k->CompID = e->CompID;
        ts = CurrentTime() + tempservetime;
        printf("\n\n\n randexp(serve) = %f\n\n\n",tempservetime);
        Schedule(ts, k);
    }
}
//state variable functions
//static 1.
int getenteredCustomers(){
    return enteredcustomer;
}
int getexitedCustomers(){
    return exitedcustomer;
}
//static 2.
double getminimumtime(){
    return minimumtime;
}
double getmaximumtime(){
    return maximumtime;
}
double getavgtime(){
    return (sumtime / exitedcustomer);
}
//static 3.
double getminimumtimeinQS(){
    return minimumtimeinQS;
}
double getmaximumtimeinQS(){
    return maximumtimeinQS;
}
double getavgtimeinQS(){
    return (sumtimeinQS / exitedcustomer);
}
// static 4.
double avgtimeofQS(int CompID){
    struct FIFOQueue* pFIFO = (struct FIFOQueue*) Component[CompID].Comp;
    return (pFIFO->totalwaitingtimeofCust / pFIFO->numberofServedCust);
}
//test main function-----------------------------------------------------------
//int main (void)
//{
//    struct EventData *d;
//    double ts;
//
//    // create components
//    MakeGenerator (0, 3.0, 1);
//    MakeExit (1);
//
//    RunSim(10.0);
//}


