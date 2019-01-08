//
//  datapsetout.h
//  6010final
//
//  Created by 刘昊 on 11/17/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#ifndef datapsetout_h
#define datapsetout_h

#include <stdio.h>
#include <stdlib.h>
int readindata(char* inputfile, char* inputfile2, char*outputfile, char*outputfile2, int** twoarrays,int* realCustNum, long* realratenum, int Mnum, int CustNum);
// this function is to read in the txt file containing customers ratings towards movies and then tranform it into a matrix
// @param inputfile: data txt file one
// @param inputfile2: data txt file two
// @param outputfile: where matrix is output
// @param outputfile2: where three parameters which is used by model part output
// @param twoarrays: a matrix with two rows, used to store relationship between index and ID
// @param realCustNum: store the real number of customers
// @param realratenum: store the total number of valid rates
// @param Mnum: maximum of movieID (typically 17770 in this program)
// @param CustNum: maximum of CustomerID(typically 30000 in this program)


void compress(char* inputfile, char* outputfile, int* C, int CustNum);// CustNum = C.length;
// this function is to readin a txt file containing the matrix of all customer towards movies and then compress this matrix, filter out invalid customers
// @param inputfile: a txt file containing matrix ouput by function "readindata"
// @param outputfile: a txt file to output the result matrix
// @param C: an array recording customers are valid or not
// @param CustNum: maximum of CustomerID(typically 30000 in this program)
#endif /* datapsetout_h */
