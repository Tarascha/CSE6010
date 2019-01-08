//
//  matrix.h
//  6140_project_2
//
//  Created by Yinquan Lu on 11/30/18.
//  Copyright © 2018 Yinquan Lu. All rights reserved.
//

#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>


double** malloc_matrix(int n1, int n2) {
    // check parameters
    if(n1 <= 0 || n2 <= 0) {
        printf("invalid parameters");
        return NULL;
    }
    // Create the variable mat, and dynamically allocate memory
    // for an array of size n1 to hold the pointers
    double** mat = (double**) malloc(n1 * sizeof(double*));
    // Check if allocation was successful by checking if malloc returned NULL
    if (mat==NULL) {
        printf("\nMemory allocation failed. Abort.\n");
        return NULL;
    }
    
    // Loop each row of the matrix and allocate an array of size n2 for each row of the matrix
    for(int i=0; i<n1; i++) {
        mat[i] = (double*) malloc(n2 * sizeof(double));
        
        // Check if allocation was successful by checking if malloc returned NULL
        if (mat==NULL) {
            printf("\nMemory allocation failed. Abort.\n");
            return NULL;
        }
    }
    
    return mat;
}


void free_matrix (int n1, int n2, double** a) {
    // check parameters
    if(n1 <= 0 || n2 <= 0 || a == NULL) {
        printf("invalid parameters");
        return;
    }
    // Loop each row and free memory
    for(int i=0; i<n1; i++) {
        double* current_row_point = a[i];
        free(current_row_point);
    }
    // Free memory for matrix point
    free(a);
    
}


void fill_matrix(int n1, int n2, double** a, int lowRank) {
    // check parameters
    if(n1 <= 0 || n2 <= 0 || a == NULL || lowRank <= 0) {
        printf("invalid parameters");
        return;
    }
    // Loop each position in matrix and assign a random value between 0.0 and upper value
    for(int i=0; i<n1; i++) {
        for(int j=0; j<n2; j++) {
            a[i][j] = (rand()/(double)RAND_MAX) / lowRank;
        }
    }
}


void print_matrix(int n1, int n2, double** a) {
    // check parameters
    if(n1 <= 0 || n2 <= 0 || a == NULL) {
        printf("invalid parameters");
        return;
    }
    // Loop each position in matrix and print it
    for(int i=0; i<n1; i++) {
        for(int j=0; j<n2; j++) {
            printf("%.5f ", a[i][j]);  //Round to one decimal when printing
        }
        printf("\n");
    }
}
