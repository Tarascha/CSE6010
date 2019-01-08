//
//  matrix_factorization.c
//  6140_project_2
//
//  Created by Yinquan Lu on 11/30/18.
//  Copyright © 2018 Yinquan Lu. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "matrix.h"
#include "matrix_factorization.h"


Parameters* factorization(Data* data_info) {
    // check parameters
    if(data_info == NULL) {
        printf("invalid parameter\n");
        return NULL;
    }
    // Give a random seed based on current time.
    int seed = (unsigned int) time(NULL);
    srand(seed);
    
    // record training time
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    Parameters* parameters;
    if ((parameters = (Parameters*) malloc (sizeof(Parameters))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
    
    rate_node* rate_list = data_info->rate_list;
    
    int num_moive = data_info->num_movie;
    int num_user = data_info->num_user;
    int num_rate = data_info->num_rate;
    
    // Parameters
    int maxIter = 200;
    double learningRate = 0.0005;
    double regularizer = 0.01;
    int lowRank = 3;
    
    // allocate memory for U, V matrices and fill them with random number between 0 and 1
    double** U;
    double** V;
    U = malloc_matrix(num_moive, lowRank);
    V = malloc_matrix(num_user, lowRank);
    fill_matrix(num_moive, lowRank, U, lowRank);
    fill_matrix(num_user, lowRank, V, lowRank);
    //    printf("print U matrix:\n");
    //    print_matrix(num_moive, lowRank, U);
    //    printf("print V matrix:\n");
    //    print_matrix(num_user, lowRank, V);
    //
    
    // Gradient Descent:
    int round = 0;  // record iteration times
    double currErr = 99999999;
    double prevErr = 999999999;  // ??? wait to be set
    
    while(round<maxIter && currErr - prevErr < -0.0001) {
        double sum_err_power = 0.0;
        
        // loop each rate
        for(int i = 0; i < num_rate; i++) {
            int moive = rate_list[i].movie;
            int user = rate_list[i].user;
            int rate = rate_list[i].rate;
            double prediction = 0.0;
            double err_for_node = 0.0;
            
            // loop U, V for certain rate to get predicition
            for(int j = 0; j < lowRank; j++) {
                prediction += U[moive][j] * V[user][j];
            }
            
            // calculate err
            err_for_node = rate - prediction;
            rate_list[i].err = err_for_node;
            sum_err_power += err_for_node * err_for_node;
        }
        
        // Stochastic GD
        // loop each rate to updata paramater in U and V
        for(int i = 0; i < num_rate; i++) {
            int moive = rate_list[i].movie;
            int user = rate_list[i].user;
            //            int rate = rate_list[i].rate;
            double err = rate_list[i].err;
            
            // update relative parameter
            for(int j = 0; j < lowRank; j++) {
                U[moive][j] = U[moive][j] + learningRate * (err * V[user][j] - regularizer * U[moive][j]);
                V[user][j] = V[user][j] + learningRate * (err * U[moive][j] - regularizer * V[user][j]);
            }
        }
        
        round += 1;
        prevErr = currErr;
        currErr = sqrt(sum_err_power / num_rate);
    }
    
//    printf("print trained U matrix:\n");
//    print_matrix(num_moive, lowRank, U);
//    printf("print trained V matrix:\n");
//    print_matrix(num_user, lowRank, V);
    
    parameters->U = U;
    parameters->V = V;
    parameters->lowRank = lowRank;
    parameters->num_moive = num_moive;
    parameters->num_user = num_user;
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("training time: %lf\n", cpu_time_used);
    
    return parameters;
}


void free_parameters(Parameters* parameters) {
    // check parameters
    if(parameters==NULL){
        printf("invalid parameters\n");
        return;
    }
    
    int lowRank = parameters->lowRank;
    int num_moive = parameters->lowRank;
    int num_user = parameters->num_user;
    
    free_matrix(num_moive, lowRank, parameters->U);  // free U
    parameters->U = NULL;
    free_matrix(num_user, lowRank, parameters->V);  // free V
    parameters->V = NULL;
    free(parameters);    
}
