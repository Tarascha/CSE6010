//
//  main.c
//  CSE6010Assignment4
//
//  Created by 刘昊 on 10/26/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define N_threads 6
#define size 900
int main(int argc, const char * argv[]) {
    // initialize;
    if(N_threads > 0){
    omp_set_num_threads(N_threads);
    }else{
        printf("Illegal number for threads\n");
        return 0;
    }
    if(size <= 0){
        printf("Illegal size for matrix\n");
        return 0;
    }
    int smallsize = size;
    int column = 0;
    double ** A = (double**)malloc(sizeof(double*)*smallsize);
    if(A == NULL){
        printf("malloc error\n");
        exit(0);
    }
    
    double **B = (double**)malloc(sizeof(double*)*smallsize);
    if(B == NULL){
        printf("malloc error\n");
        exit(0);
    }
    for(int i = 0; i < smallsize; i++){
        A[i]=(double*)malloc(smallsize * sizeof(double));
        B[i]=(double*)malloc(smallsize * sizeof(double));
        if(A[i] == NULL || B[i] == NULL){
            printf("malloc error\n");
            exit(0);
        }
    }
    
    //double shared_C[smallsize][smallsize];
    double ** C = (double**)malloc(sizeof(double*)*smallsize);
    if(C == NULL){
        printf("malloc error\n");
        exit(0);
    }
    for(int i = 0; i < smallsize; i++){
        C[i]=(double*)malloc(smallsize * sizeof(double));
        if(C[i] == NULL){
            printf("malloc error\n");
            exit(0);
        }
    }
    srand((unsigned)time(NULL));
    for(int i = 0; i < smallsize; i++){
        for(int j = 0; j < smallsize; j++){
            A[i][j] = rand()/(double)(RAND_MAX/1);
            B[i][j] = rand()/(double)(RAND_MAX/1);
            //shared_C[i][j] = 0.0;
        }
    }
    // static parallel-----------------------------------
    double start_static = omp_get_wtime();
#pragma omp parallel
    {
        int ID = omp_get_thread_num();
        printf("threadnumber:%d\n",ID);
        
        double ** Local_C = (double**)malloc(sizeof(double*)*smallsize);
        if(Local_C == NULL){
            printf("malloc error\n");
            exit(0);
        }
        for(int i = 0; i < smallsize; i++){
            Local_C[i]=(double*)malloc(smallsize * sizeof(double));
            if(Local_C[i] == NULL){
                printf("malloc error\n");
                exit(0);
            }
        }
        
        for(int i = 0; i < smallsize; i++){
            for(int j = 0; j < smallsize; j++){
                Local_C[i][j] = 0.0;
            }
        }
        for(int i = ID; i < smallsize; i+= N_threads){// c's column
            for(int j = 0; j < smallsize; j++){//A's row
                for(int k = 0; k < smallsize; k++){//B's column
                    Local_C[j][i] += A[j][k] * B[k][i];
                }
            }
        }
    #pragma omp critical
        {
    //
            for(int i = ID; i < smallsize; i+= N_threads){
                for(int j = 0; j < smallsize; j++){
                    C[j][i] = Local_C[j][i];
                }
            }
    }
        for(int i = 0; i < smallsize; i++){
            free(Local_C[i]);
        }
        free(Local_C);
        
}
    double delta_static = omp_get_wtime() - start_static;
    printf("matrix C for static parallel:\n");
    for(int i = 0; i < smallsize; i++){
        for(int j = 0; j < smallsize; j++){
            printf("%f\t",C[i][j]);
            
        }
        printf("\n");
    }
    printf("Time taken for static parallel = %f\n", delta_static);
    //sequential--------------------------------------------------
    for(int i = 0; i < smallsize; i++){
        for(int j = 0; j < smallsize; j++){
            
            C[i][j] = 0.0;
        }
    }
    double start_sequential = omp_get_wtime();
    for(int i = 0; i < smallsize; i++){// c's column
        for(int j = 0; j < smallsize; j++){//A's row
            for(int k = 0; k < smallsize; k++){//B's column
                C[j][i] += A[j][k] * B[k][i];
            }
        }
    }
    double delta_sequential = omp_get_wtime() - start_sequential;
    printf("matrix C for sequential parallel:\n");
    for(int i = 0; i < smallsize; i++){
        for(int j = 0; j < smallsize; j++){
            printf("%f\t",C[i][j]);
            
        }
        printf("\n");
    }
    printf("Time taken for sequential = %f\n", delta_sequential);
    printf("Time taken for static parallel = %f\n", delta_static);
    
    //dynamic parallel----------------------------------
    for(int i = 0; i < smallsize; i++){
        for(int j = 0; j < smallsize; j++){
            
            C[i][j] = 0.0;
        }
    }
    double start_dynamic = omp_get_wtime();
    
    
    
        #pragma omp parallel
                {
                    while(column < smallsize){
                    //int ID = omp_get_thread_num();
                    int cur_column = 0;
                    double *local_CC = (double*)malloc(sizeof(double)*smallsize);
                    double local_cc = 0;
                    //printf("ID:%dstep 1\n",ID);
        #pragma omp critical
                    {
                            cur_column = column;
                            column++;
                        //printf("ID:%dstep 2\n",ID);
                    }
                    //printf("ID:%dstep 3\n",ID);
                    for(int k = 0; k < smallsize; k++){
                        for(int i = 0; i < smallsize; i++){
                            local_cc += A[k][i] * B[i][cur_column];
                        }
                        local_CC[k] = local_cc;
                        local_cc = 0.0;
                    }
                    //printf("ID:%dstep 4\n",ID);
        #pragma omp critical
                    {
                        //printf("ID:%dstep 5\n",ID);
                        for(int i = 0; i < smallsize; i++){
                            C[i][cur_column] = local_CC[i];
                            //printf("local_CC = %f\n",local_CC[i]);
                        }
                    }
                    //printf("ID:%dstep 6\n",ID);
                        free(local_CC);
                }
    }
    double delta_dynamic = omp_get_wtime() - start_dynamic;
    printf("matrix C for dynamic parallel:\n");
    for(int i = 0; i < smallsize; i++){
        for(int j = 0; j < smallsize; j++){
            printf("%f\t",C[i][j]);

        }
        printf("\n");
    }
    printf("Time taken for sequential = %f\n", delta_sequential);
    printf("Time taken for static parallel = %f\n", delta_static);
    printf("Time taken for dynamic parallel = %f\n", delta_dynamic);
    //free all matrix allocated--------------
    for(int i = 0; i < smallsize; i++){
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
        free(A);
        free(B);
        free(C);
    
    return 0;
}
