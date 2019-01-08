//
//  assignment2_26010.c
//  assignment2_16010
//
//  Created by 刘昊 on 9/23/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#include "assignment2_26010.h"


nodes**  readinfile(char* inputfile, int* cc){//char*outputfile,cc is the count number;
    
    FILE* ifp;
    ifp = fopen(inputfile, "r");// input graphfile
    if(ifp == NULL){
        printf("none exist file\n");
        fclose(ifp);
    }
    char str0[100] ;// get how many nodes are in the graph;
    fgets(str0, 100, ifp);
    int count = atoi(str0);
    int i = 0;
//    printf("number of counts:%d\n",count);
    
    char** str = (char**)malloc(count*sizeof(char*));//build a matrix str to store all the informatio
    if(str == NULL){
        printf("error,lacking memory");
        return NULL ;
    }
    for(i = 0; i < count; i++){
        str[i] = (char*)malloc(100*sizeof(char));
        if(str[i] == NULL){
            printf("error,lacking memory");
           return  NULL ;
        }
    }// builded;
    
    for(i = 0; i < count; i++){//write information in str
        fgets(str[i], 100, ifp);
    }
    
//    for(i = 0; i < count; i++){// print to check
//        int j = 0;
//        while(str[i][j] != '\n'){
//            printf("%c",str[i][j]);
//            j++;
//        }
//        printf("\n");
//    }//checked right!
    
    //now starts to convert str to a linked list!!!!!
    nodes** A = (nodes**)malloc(count*sizeof(nodes*));//A is the general pointer;
    if(A == NULL){
        printf("not enough memory for A\n");
        return NULL;
    }
    for(i = 0; i < count; i++){// implement the linked list;
        nodes* newnode = malloc(sizeof(nodes));
        if(newnode == NULL){
            printf("not enough memory for newnode\n");
            return NULL;
        }
        char newstr[count][100]; // to convert str's node number into int;
        for(int u = 0; u < count; u++){
            for(int p = 0; p < 100; p++){
                newstr[u][p] = 'x';
            }
        }//initialize;
        int k = 0,j = 0;
        while(str[i][k] >= '0' && str[i][k] <= '9'){
            newstr[j][k] = str[i][k];
            k++;
        }// now k pointes to the first blank;
        newnode->data = atoi(newstr[j]);// original node number;
        newnode->next = NULL;
        A[i] = newnode;// first column of linked list builded!;
        if(str[i][k + 1] == '\n'){// when k+1 points to /n
            continue;
        }
        nodes* temp = A[i];// temp is for insertion next node;
        //  put newstr to the next row, for the next node number linked with original node;
        for(j = 1; j< count; j++){
            int t =0;
            nodes* new2node = malloc(sizeof(nodes));
            if(new2node == NULL){
                printf("not enough memory for new2node\n");
                return NULL;
            }
            k++;// k from blank to next number ;
            while(str[i][k] >= '0' && str[i][k] <= '9'){// put second number into newstr;
                newstr[j][t] = str[i][k];// k is not 0 for newstr any more
                k++;
                t++;
            }// now k points to a blank ;
           
            new2node->data = atoi(newstr[j]);
            new2node->next = NULL;
            temp->next = new2node;
            temp = new2node;
            if(str[i][k + 1] == '\n'){// when k + 1 points to a /n, break this j loop,
                break;
            }
        }
    }// lingked list builded!!!!!!!
    fclose(ifp);
    for(i = 0;i < count;i++){
        free(str[i]);
    }
    free(str);
    *cc = count;
    return A;
}

void printlinkedlist (nodes** A, int cc){
    int i = 0;
    for(i = 0; i < cc; i++){
        nodes* temp = A[i];
        while(temp != NULL){
            printf("%d\t",temp->data);
            temp = temp->next;
        }
        printf("\n");
    }
}
void freelinkedlist(nodes **A, int cc){
    int i = 0;
    for(i = 0; i < cc;i++){
        nodes*temp = A[i];
        nodes*temp2;
        while(temp != NULL){
            temp2 = temp;
            temp = temp->next;
            free(temp2);
        }
    }
    free(A);
}


void analyse(char* c, nodes** A, int cc, int color[]){//char* c,
    FILE* ofp;
    ofp = fopen(c, "w");
    int textcount = 0;//count how many components
    int componentselements[cc];//count how many elements in one component;
    for(int i = 0; i < cc; i++){
        componentselements[i] = 0;//set 0;
    }
    int t = 0; //t is for the componentselements[];
    for(int i = 0; i < cc; i++){
        if(color[i] == 0){
            travel(A, i,color,&componentselements[t]);
            textcount ++;
            t++;
            printf("next travel:\n");
        }
    }//travel done, compoelements[0~textcount-1] stored every components' elements number;
    //for example, componentelement[0] store number of traveled nodes from node 0;
    //componentelement[1] stored number of next travel nodes;
    int sum = 0;
    int maxcomponentnumber = 0;
    
    for(int i= 0; i < textcount;i++){
        sum += componentselements[i];
        if(componentselements[i] > maxcomponentnumber){
            maxcomponentnumber = componentselements[i];
        }
    }
    
    int statisticarray[maxcomponentnumber][2];//classify every components;
    for(int i = 0; i < maxcomponentnumber; i++){
        statisticarray[i][0] = i + 1;
        statisticarray[i][1] = 0;
        //printf("%d\t%d\n",statisticarray[i][0],statisticarray[i][1]);//check
    }// initialise for statistic array;
    for(int i = 0; i < textcount; i++){// go over the componentelement[]
        statisticarray[componentselements[i] - 1][1] ++;
    }
    
    fprintf(ofp, "%d\n",textcount);
    fprintf(ofp, "%f\n",(float)sum/textcount);
    fprintf(ofp, "%d\n",maxcomponentnumber);
    printf("maxcomponentsize:%d\n",maxcomponentnumber);
    for(int i =0; i < maxcomponentnumber; i++){
        fprintf(ofp,"%d\t%d\n",statisticarray[i][0],statisticarray[i][1]);
    }
    fclose(ofp);
    
}

void travel(nodes** A, int i, int color[], int *componentelement){//DFS
    if(color[A[i]->data] == 1){
        return ;
    }
    color[A[i]->data] = 1;
    *componentelement += 1;
    printf("%d\t",A[i]->data);
    nodes* temp = A[i];
    while(temp->next != NULL){
        travel( A, temp->next->data,color,componentelement);
        temp = temp->next;
    }
}
