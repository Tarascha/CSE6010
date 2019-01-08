//
//  assignment1c.c
//  assignment1C
//
//  Created by 刘昊 on 9/11/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#include "assignment1c.h"
PrioQ* PQ_creat(void){
    PrioQ* headpointer=malloc(sizeof(PrioQ));
    if(headpointer==NULL){
        printf("Error, since lacking of memory\n");
        return NULL;
    }
    
    return headpointer;
}
int PQ_insert(PrioQ* PQ, double key, void* data){
    if(PQ==NULL||data==NULL){
        printf("Error,invalid data");
        return -1;
    }
    nodes *newnode=malloc(sizeof(nodes));
    newnode->Data=data;
    newnode->next=NULL;
    newnode->key=key;
    if(PQ->head==NULL){
    PQ->head=newnode;
    PQ->tail=newnode;
        return 0;
    }
    nodes* temp=PQ->head;
    if(PQ->head->key>key){
        PQ->head=newnode;
        newnode->next=temp;
        return 0;
    }
    while(temp->key<=key){
        PQ->tail=temp;
        temp=temp->next;
        if(temp==NULL)
            break;
        
    }
        newnode->next=temp;
        PQ->tail->next=newnode;
    PQ->tail=PQ->head;//this step is used for PQfree();
    return 0;
}
void* PQ_delete(PrioQ* PQ, double *key){
    if(PQ->head==NULL){
        printf("the queue is empty\n");
        return NULL;
    }
   
    void* rdata=PQ->head->Data;
    *key=PQ->head->key;
    PQ->head=PQ->head->next;
    return rdata;
}

 unsigned int PQ_count(PrioQ* PQ){
    if(PQ->head==NULL){
        return 0;
    }
    nodes* temp=PQ->head;
    int i=0;
    while(temp!=NULL){
        i++;
        temp=temp->next;
    }
    return i;
}
void* PQfree(PrioQ* PQ){
    if (PQ == NULL){
        printf ("it is an empty PQ\n");
    }
    nodes* temp;
    while(PQ->tail!=NULL){
        temp=PQ->tail;
        PQ->tail=PQ->tail->next;
        free(temp);
    }
    free(PQ->head);
    return NULL;
}
