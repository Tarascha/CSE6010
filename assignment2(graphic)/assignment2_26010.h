//
//  assignment2_26010.h
//  assignment2_16010
//
//  Created by 刘昊 on 9/23/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#ifndef assignment2_26010_h
#define assignment2_26010_h

#include <stdio.h>
#include <stdlib.h>
//#include <MacTypes.h>
typedef struct Nodes{
    int data;
    struct Nodes* next;
}nodes;

void colored(void);
nodes** readinfile(char* inputfile, int* cc);
void printlinkedlist (nodes** A, int cc);
void freelinkedlist(nodes **A, int cc);
void travel(nodes** A, int i, int color[],int *componentelement);
void analyse(char* c,nodes** A, int cc,int color[]);//char* c, 
#endif /* assignment2_26010_h */
