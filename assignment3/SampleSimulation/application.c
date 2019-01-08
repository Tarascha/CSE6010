#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sim.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//
// One Pump Gas Station Simulation
// Arriving vehicles use the gas pump; queue if pump being used
//
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////
//
// State variables  and other global information
//
/////////////////////////////////////////////////////////////////////////////////////////////

int	AtPump=0;	// #vehicles at the pump or waiting to use it; 0 if pump is free

// Event types
#define	ARRIVAL     1
#define	DEPARTURE   2


/////////////////////////////////////////////////////////////////////////////////////////////
//
// Data structures for event data
//
/////////////////////////////////////////////////////////////////////////////////////////////
//
// Each event can have any number of parameters, each of arbitrary data type. The number and types are
// dependent on the simulation application. This information is defined here. This information is
// stored in each event that is scheduled. Note that the simulation engine does not need to know
// the number and type of the parameters, so this information is hidden from the engine. The simulation
// engine is only given a pointer to event parameters.
// For this simple application, events only have one parameter, indicating the kind of event
// (ARRIVAL or DEPARTURE)

struct EventData {
	int EventType;
};

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Function prototypes
//
/////////////////////////////////////////////////////////////////////////////////////////////

// prototypes for event handlers
void Arrival (struct EventData *e);		// car arrival event
void Departure (struct EventData *e);	// car departure event

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

// General Event Handler Procedure define in simulation engine interface
// This function is called by the simulation engine to process an event removed from the future event list
void EventHandler (void *data)
{
    struct EventData *d;
    
    // coerce type so the compiler knows the type of information pointed to by the parameter data.
    d = (struct EventData *) data;
    // call an event handler based on the type of event
    if (d->EventType == ARRIVAL) Arrival (d);
    else if (d->EventType == DEPARTURE) Departure (d);
    else {fprintf (stderr, "Illegal event found\n"); exit(1); }
    free (d); // Release memory for event paramters
}

// event handler for arrival events
void Arrival (struct EventData *e)
{
	struct EventData *d;
	double ts;

    printf ("Processing Arrival event at time %f, AtPump=%d\n", CurrentTime(), AtPump);
	if (e->EventType != ARRIVAL) {fprintf (stderr, "Unexpected event type\n"); exit(1);}

	// schedule next arrival event, here, 10 time units from now
    if((d=malloc(sizeof(struct EventData)))==NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
    d->EventType = ARRIVAL;
    ts = CurrentTime() + 10.0;
    Schedule (ts, d);

    // if the pump is free, vehicle will use the pump for 15 time unts; schedule departure event
	if (AtPump == 0) {
		if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
		d->EventType = DEPARTURE;
		ts = CurrentTime() + 16.0;
		Schedule (ts, d);
	}
    AtPump++;   // update state variable
}

// event handler for departure events
void Departure (struct EventData *e)
{
    struct EventData *d;
    double ts;
    
    printf ("Processing Departure event at time %f, AtPump=%d\n", CurrentTime(), AtPump);
	if (e->EventType != DEPARTURE) {fprintf (stderr, "Unexpected event type\n"); exit(1);}

	AtPump--;   // one fewer vehicle at pump

    // If another vehicle waiting to use pump, allocate pump to vehicle, schedule its departure event
    if (AtPump>0) {
        if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
        d->EventType = DEPARTURE;
        ts = CurrentTime() + 16.0;
        Schedule (ts, d);
    }
}

///////////////////////////////////////////////////////////////////////////////////////
//////////// MAIN PROGRAM
///////////////////////////////////////////////////////////////////////////////////////

int main (void)
{
	struct EventData *d;
	double ts;

	// initialize event list with first arrival
	if ((d=malloc (sizeof(struct EventData))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
	d->EventType = ARRIVAL;
	ts = 10.0;
	Schedule (ts, d);

	RunSim(50.0);
}
