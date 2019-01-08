#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sim.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Queueing Network Simulation derived from Sample Gas Station Simulation
//
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Data structures for event data
//
/////////////////////////////////////////////////////////////////////////////////////////////

// EVENT TYPES:
//  GENERATE: Generation of a new customer
//      Parameters: Component where customer is generated
//  ARRIVAL: Customer arrives at a queue station, fork, or exit component
//      Parameters: Pointer to arriving customer and component where customer arrives
//  DEPARTURE: a customer departs from a queue station
//      Parameters: ...

// Event types: Generate new customer, arrival at a component, departure from queue station
#define     GENERATE    0
#define     ARRIVAL     1
#define     DEPARTURE   2

// A single data structure is used to handle all three event types
struct EventData {
    int EventType;              // Type of event (GENERATE, ARRIVAL, DEPARTURE)
    struct Customer *Cust;      // Arriving or departing customer; unused for GENERATE events
    int CompID;                 // ID of component where customer created, arrives, or departs
};

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Data Structures for Customers and FIFO queue
//
/////////////////////////////////////////////////////////////////////////////////////////////

// Customers
struct Customer {
    double CreationTime;        // time customer was created
    double ExitTime;            // time customer departs system
    struct Customer *next;      // pointer to next customer when it is queued in a FIFO
};

// Linked List of customers (FIFO Queue)
struct FIFOQueue {
    struct Customer *first;     // pointer to first customer in queue
    struct Customer *last;      // pointer to last customer in queue
};

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Data Structures for Components
//
/////////////////////////////////////////////////////////////////////////////////////////////

// maximum number of components that can be defined for the simulation
#define MAXCOMPONENTS    100

// Component types
#define     GENERATOR       0
#define     FORK            1
#define     QUEUE_STATION   2
#define     EXIT            3
//
// Generator Component
//
struct Generator {
    double IntArrTime;     // mean interarrival time for generated components
    int DestComp;          // ID of next component customers are sent to
};

//
// Exit component
//
struct Exit {
    int Count;              // number of customers that exited at this component
};

// global, master list of components
struct {
    int ComponentType;  // GENERATOR, QUEUE_STATION, FORK, EXIT
    void *Comp;         // Pointer to information on component (Generator, Exit struct, etc.)
} Component[MAXCOMPONENTS];

//
// Functions to create new components
//

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

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Function prototypes
//
/////////////////////////////////////////////////////////////////////////////////////////////

// prototypes for event handlers
void Generate (struct EventData *e);    // generate new customer
void Arrival (struct EventData *e);        // arrival at a component

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Event Handlers
// Parameter is a pointer to the data portion of the event
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Note the strategy used for dynamic memory allocation. The simulation application is responsible
// for freeing any memory it allocates, and is ONLY responsible for freeing memory allocated
// within the simulation application (i.e., it is NOT responsioble for freeing memory allocated
// within the simulation engine). Here, the simulation dynamically allocates memory
// for the PARAMETERS of each event. Whenever a new event is scheduled, memory for the event's paramters
// is allocated using malloc. This storage is released within the event handler when it is done
// processing the event.
// Because we know each event is scheduled exactly once, and is processed exactly once, we know that
// memory dynamically allocated for each event'a parameters will be released exactly once.
// Note that within the simulation engine memory is also dynamically allocated and released. That memory
// is different from the memory allocated here, and the simulation application is not concerned with
// memory allocated in the simulation engine.
//

// General Event Handler Procedure defined in simulation engine interface called by simulation engine
void EventHandler (void *data)
{
    struct EventData *d;
    
    // coerce type so the compiler knows the type of information pointed to by the parameter data.
    d = (struct EventData *) data;
    // call an event handler based on the type of event
    if (d->EventType == GENERATE) Generate (d);
    else if (d->EventType == ARRIVAL) Arrival (d);
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
    NewCust->CreationTime = CurrentTime();
    NewCust->ExitTime = 0.0;    // undefined initially
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
    ts = CurrentTime() + pGen->IntArrTime;       // need to modify to exponential random number
    Schedule (ts, d);
}

// event handler for arrival events
void Arrival (struct EventData *e)
{
    struct EventData *d;
    double ts;
    int Comp;                   // ID of component where arrival occurred
    struct Exit *pExit;         // pointer to info on exit component
    
    if (e->EventType != ARRIVAL) {fprintf (stderr, "Unexpected event type\n"); exit(1);}
    printf ("Processing Arrival event at time %f, Component=%d\n", CurrentTime(), e->CompID);
    
    // processing depends on the type of component arrival occurred at
    if (Component[e->CompID].ComponentType == EXIT) {
        pExit = (struct Exit *) Component[e->CompID].Comp;
        (pExit->Count)++;       // number of exiting ßcustomers
        free (e->Cust);         // release memory of exiting customer
    }
    else if (Component[Comp].ComponentType == QUEUE_STATION) {
        // code for customer arrival at a queue station
    }
    else if (Component[Comp].ComponentType == FORK) {
        // code for customer arrival at a fork component
    }
    else {fprintf (stderr, "Bad component type for arrival event\n"); exit(1);}
}

///////////////////////////////////////////////////////////////////////////////////////
//////////// MAIN PROGRAM
///////////////////////////////////////////////////////////////////////////////////////

int main (void)
{
    struct EventData *d;
    double ts;
    
    // create components
    MakeGenerator (0, 3.0, 1);
    MakeExit (1);
    
    RunSim(10.0);
}
