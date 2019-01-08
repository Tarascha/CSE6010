//
//  main.c
//  6010A3
//
//  Created by 刘昊 on 10/9/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "6010A3_library.h"


int main(int argc, const char * argv[]) {
    srand((unsigned)time(NULL));
//    int ID[] = {3,4,5,1};
//    double possb[] = {0.4,0.4,0.1,0.1};
//    MakeGenerator(0,3.0,1);
//    MakeQS(1,10.0,2);
//    MakeFork(2,4,ID,possb);
//    MakeQS(3,7.0,1);
//    MakeQS(4,5.0,1);
//    MakeExit (5);
//    RunSim(50);
    
//    int ID[] = {3,4,5,1};
//    double possb[] = {0.4,0.4,0.1,0.1};
//    MakeGenerator(0,3.0,1);
//    MakeQS(1,4.0,2);
//    MakeFork(2,4,ID,possb);
//    MakeQS(3,5.0,1);
//    MakeQS(4,6.0,1);
//    MakeExit (5);
//    RunSim(50.0);
    MakeGenerator(0, 6, 1);
    MakeQS(1, 2, 3);
    MakeExit(3);
    RunSim(50);
    
    printf(" numnber of entered customers:%d\n",getenteredCustomers());
    printf("number fo exited customers:%d\n",getexitedCustomers());
    printf("minimum time of a customer stay in the system:%f\n",getminimumtime());
    printf("maximum time of a customer stay in the system:%f\n",getmaximumtime());
    printf("avg time of a customer stay in the system:%f\n",getavgtime());
    printf("minimum time of a customer stay in all the QS:%f\n",getminimumtimeinQS());
    printf("maximum time of a customer stay in all the QS:%f\n",getmaximumtimeinQS());
    printf("avg time of a customer stay in all the QS:%f\n",getavgtimeinQS());
    printf("avg waiting time experienced by a customer in QS%d:%lf\n",1,avgtimeofQS(1));
//    printf("avg waiting time experienced by a customer in QS%d:%lf\n",3,avgtimeofQS(3));
//    printf("avg waiting time experienced by a customer in QS%d:%lf\n",4,avgtimeofQS(4));
    return 0;
}
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <time.h>
//#include <string.h>
//
//#include "6010A3_library.h"
//
//#define buflen 1000
//
////////////////////////////////////////////////////////////////////////////
////
//// Structs
////
////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////
////
//// Discrete Event Simulator: Cafeteria
////
////////////////////////////////////////////////////////////////////////////
//
//void getConfig(FILE*, int);
//
//
////////////////////////////////////////////////////////////////////////////
////
//// Supporting Functions
////
////////////////////////////////////////////////////////////////////////////
//void getConfig(FILE* in,int n){
//
//    char type = '-';
//    int id;
//    char buf[buflen];
//    char c;
//
//    double p;
//    int exitport;
//    int k;
//
//    double *probs;
//    int *outs;
//
//    double totalprob=0;
//    int running_id;
//
//
//
//    running_id = 0;
//    for(int total=0; total<n; total++){
//        fscanf(in, "%d %c", &id, &type);
//        printf("%d %c\n", id, type);
//
//        //check ordering of id
//        if(id != running_id){
//            printf(" ERROR: INCORRECT FILE FORMAT (ID) \n");
//            exit(-1);
//        }
//        running_id++;
//
//        // GENERATOR
//        if(type == 'G'){
//            fscanf(in, "%lf %d\n", &p, &exitport);
//
//            //printf("av time: %f\nout: %d\nn: %d", p, exitport, n);
//            // check valid time and exitport
//            if((p <= 0) || (exitport > (n-1))){
//                printf(" ERROR: INCORRECT FILE FORMAT (GENERATOR) \n");
//                exit(-1);
//            }
//
//            // make generator
//            MakeGenerator(id, p, exitport);
//
//
//            // QUEUE
//        } else if(type == 'Q'){
//            fscanf(in, "%lf %d\n", &p, &exitport);
//            printf("\tQUEUE: av time: %lf\tout: %d\n", p, exitport);
//
//            // check valid time and exitport
//            if((p <= 0) || (exitport > (n-1))){
//                printf(" ERROR: INCORRECT FILE FORMAT (QUEUE) \n");
//                exit(-1);
//            }
//
//            // make queue
//            MakeQS(id, p, exitport);
//
//            // EXIT
//        } else if(type == 'X'){
//            MakeExit (id);
//
//            // FORK
//        } else if(type == 'F'){
//            fscanf(in, "%d ", &k);
//
//            if ((probs=malloc(k*sizeof(double))) == NULL) {fprintf(stderr, "malloc error\n"); exit(-1);}
//            if ((outs=malloc(k*sizeof(int))) == NULL) {fprintf(stderr, "malloc error\n"); exit(-1);}
//
//            totalprob = 0;
//            for(int i=0; i<k; i++){
//                fscanf(in, "%lf ", &probs[i]);
//                printf("p: %lf ", probs[i]);
//                totalprob += probs[i];
//            }
//
//            printf("\n");
//
//            //check that probabilities make sense
//            if(totalprob != 1.0){
//                printf(" ERROR: INCORRECT FILE FORMAT (FORK) \n");
//                exit(-1);
//            }
//
//            for(int j=0; j<k; j++){
//                fscanf(in, "%d ", &outs[j]);
//                printf("out: %d ", outs[j]);
//            }
//
//            // make fork
//            MakeFork(id, k, outs, probs);
//
//            free(probs);
//            free(outs);
//        }
//        printf("\n");
//    }
//
//    c=fgetc(in); //eat newline
//    c=fgetc(in); //eat eof
//
//
//    // if theres more left after the designated number of inputs, something's wrong'
//    if(!feof(in)){
//        printf(" ERROR: FILE FORMAT ERROR (INCONSISTENT NUMBER OF ELEMENTS/BLANK SPACE) \n");
////        exit(-1);
//    }
//
//}
//
//int main(int argc, const char* argv[]){
//
//    /*    srand((unsigned)time(NULL));*/
//    srand(time(0));
//
//    struct EventData *d;
//    double ts;
//    int n_elements;
//    char tmp;
//
//    // check function call
//    if(argc != 4){
//        printf(" ERROR: INCORRECT CALL FORMAT \n");
//        printf("format: ./cpssim <time> <config file> <output file>\n");
//        exit(0);
//    }
//
//    // open config file
//    FILE *in = NULL;
//    if((in = fopen(argv[2],"r")) == NULL){
//        printf(" ERROR: FILE OPEN ERROR \n");
//        exit(-1);
//    }
//
//    // read config file
//    //get number of elements
//    if((fscanf(in, "%d", &n_elements)) == EOF){
//        printf(" ERROR: FILE FORMAT ERROR \n");
//        exit(0);
//    }
//
//    // eat newline
//    tmp = fgetc(in);
//    getConfig(in, n_elements);
//
//    fclose(in);
//
//    // run simulation
//    RunSim(atoi(argv[1]));
//
//
//}
