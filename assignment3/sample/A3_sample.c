#include <stdio.h>

// library entity
typedef struct generator{
    int id;
    float arrival_time;
    int output_port;
    ...
}Gen;

//library entity
typedef struct customer{
    float arrival_time;
    float dep_time;
    ....
}Customer;

//library entity
typedef struct queueing_station{
    Q queue; // Linked List of customer
    size;
    etc...
}QS;

// Linked List of customer
typedef struct Queue
{
    node;
    next;
}Q;

// library function
Gen* BuildGenerator(int id, float arrival time, int output_port)
{
    malloc;
    assign;
    Schedule seed generator event;
    return;
}


Events
// library
void GeneratorEvent( Gen* G )
{
    DoJob//
    create customer -> send him to entry QS
    Schedule next generator Event;
}

// library
void finishserve( QS *QS )
{
    depart from current station;
    send him to next destination; // QS or fork
    Check for Customers in your Q schedule next serve events;
}

// Auxillary Function library
void AcceptCustomers( QS *QS )
{
    check if Q is empty then schedule serve;
    else add to Q;
}




