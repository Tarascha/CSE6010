//
//  matrix.c
//  6140_project_2
//
//  Created by Yinquan Lu on 11/30/18.
//  Copyright © 2018 Yinquan Lu. All rights reserved.
//

#ifndef SRC_ASSIGNMENT1B_H_
#define SRC_ASSIGNMENT1B_H_

/*
 * Creates a two-dimensional matrix using dynamic memory allocation.
 *
 * @param n1: row number
 * @param n2: column number
 * @return a point to a n1*n2 matrix if successful, and NULL on error.
 */
double **malloc_matrix(int n1, int n2);


/*
 * Release the memory allocated for the matrix pointed to.
 *
 * @param n1: row number
 * @param n2: column number
 * @param a: the point for matrix
 */
void free_matrix (int n1, int n2, double **a);


/*
 * Fill the matrix with random values between 0.0 and 10.0.
 *
 * @param n1: row number
 * @param n2: column number
 * @param a: the point for matrix
 */
void fill_matrix(int n1, int n2, double** a, int lowRank);


/*
 * print the matrix.
 *
 * @param n1: row number
 * @param n2: column number
 * @param a: the point for matrix
 */
void print_matrix(int n1, int n2, double** a);


/*
 * Multiply two matrix.
 *
 * @param n1: row number for matrixA
 * @param n2: column number for matrixA and row number for matrixB
 * @param n3: column number for matrixB
 * @param a, b, c: the point for matrixA, matrixB, matrixC
 */
int matrix_multiply(int n1, int n2, int n3, double** a, double** b, double** c);

#endif /* SRC_ASSIGNMENT1B_H_ */
