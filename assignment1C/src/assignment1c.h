//
//  assignment1c.h
//  assignment1C
//
//  Created by 刘昊 on 9/11/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#ifndef assignment1c_h
#define assignment1c_h

#include <stdio.h>
#include <sys/malloc.h>
#include <stdlib.h>
#include <time.h>
typedef struct Nodes{
    double key;
    void* Data;
    struct Nodes* next;
}nodes;
typedef struct Prioq{
    nodes* head;
    nodes* tail;
}PrioQ;
PrioQ* PQ_creat(void);
int PQ_insert(PrioQ* PQ, double key, void* data);
void* PQ_delete(PrioQ* PQ,double *key);
unsigned int PQ_count(PrioQ* PQ);
void* PQfree(PrioQ* PQ);
#endif /* assignment1c_h */
