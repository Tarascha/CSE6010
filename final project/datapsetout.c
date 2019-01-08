//
//  datapsetout.c
//  6010final
//
//  Created by 刘昊 on 11/17/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#include "datapsetout.h"
int readindata(char* inputfile, char* inputfile2, char*outputfile, char* outputfile2, int** twoarrays,int* realCustNum, long* realratenum, int Mnum, int CustNum){// CustNum 不要超过65535/2
    //    long movienumber = 17770;
    //    long customernumber = 2649429;
    int movienumber = Mnum;
    int customernumber = CustNum;
    FILE* ifp;
    FILE* ifp2;
    FILE* ofp;
    FILE * ofp2;
    ifp = fopen(inputfile, "r");
    if(ifp == NULL){
        printf("inputfile does not exist\n");
        fclose(ifp);
        exit(1);
    }
    ifp2 = fopen(inputfile2, "r");
    if(ifp2 == NULL){
        printf("inputfile2 does not exist\n");
        fclose(ifp2);
        exit(1);
    }
    ofp = fopen(outputfile, "w");
    ofp2 = fopen(outputfile2, "w");
    
    
    //    for(long i = 1; i <= movienumber; i++){
    //        matrix[0][i] = i; // movieID
    //    }
    //    for(long j = 1; j <= customernumber; j++){
    //        matrix[j][0] = j;
    //    }
    
    int* matrix = (int*)malloc(customernumber*sizeof(int));//all customers' rating to a movie
    if(matrix == NULL){
        printf("not enough memory for matrix\n");
        exit(1);
    }
    for(int i = 0; i < customernumber; i++){
        matrix[i] = 0;
    }
    int* movieindex = (int*)malloc(movienumber*sizeof(int));
    if(movieindex == NULL){//store movieId
        printf("not enough memory for movieindex\n");
        exit(0);
    }
    for(int i = 0; i < movienumber; i++){
        movieindex[i] = 0;
    }
    int* CustomerNumber = (int*)malloc(customernumber*sizeof(int));
    if(CustomerNumber == NULL){//store movieId
        printf("not enough memory for CustomerNumber\n");
        exit(0);
    }
    for(int i = 0; i < customernumber; i++){
        CustomerNumber[i] = -1;
    }
    char s[100];
    int movieId = 0;
    int movierow = 0; // movies index in matrix// also number of realmoives
    int customerId = 0;
    int realcustomerNum = 0;
    long rateNum = 0;
    int rate = 0;
    while((fgets(s,100,ifp))!=NULL){
        long number = 0;
        int i = 0;
        while(s[i] != ',' && s[i] != ':'){
            number = number * 10 + (long)(s[i] - '0');
            i++;
        }// i point to ','
        if(s[i] == ':'){
            if(movierow != 0){
                for(int t = 0; t < customernumber; t++){
                    if(t != customernumber - 1){
                        fprintf(ofp, "%d\t", matrix[t]);
                    }else{
                        fprintf(ofp, "%d\n", matrix[t]);
                    }
                }
                free(matrix);
                matrix = (int*)malloc(customernumber*sizeof(int));
                if(matrix == NULL){
                    printf("not enough memory for matrix\n");
                    exit(0);
                }
                for(int t = 0; t < customernumber; t++){
                    matrix[t] = 0;
                }
            }// won't fprintf in first round
            movieId = (int)number;
            movieindex[movierow] = movieId;
            movierow++;
            continue;
        }else{
            if(number < CustNum){
                customerId = (int)number;
                if(CustomerNumber[customerId - 1] == -1){
                    CustomerNumber[customerId - 1] = realcustomerNum;
                    realcustomerNum++;
                }
                i++;
                rateNum ++;
                rate = (int)(s[i] - '0');
                if(rate > 5 || rate < 0){
                    rate = 0; //data clean
                    rateNum --;
                }
                matrix[customerId - 1] = rate;
            }
            
        }
        
    }
    int movierow1 = movierow;
    //fprintf last movie's information;
    for(int t = 0; t < customernumber; t++){
        if(t != customernumber - 1){
            fprintf(ofp, "%d\t", matrix[t]);
        }else{
            fprintf(ofp, "%d\n", matrix[t]);
        }
    }
    free(matrix);
    //inputfile2
    matrix = (int*)malloc(customernumber*sizeof(int));
    if(matrix == NULL){
        printf("not enough memory for matrix\n");
        exit(0);
    }
    for(int t = 0; t < customernumber; t++){
        matrix[t] = 0;
    }
    while((fgets(s,100,ifp2))!=NULL){
        long number = 0;
        int i = 0;
        while(s[i] != ',' && s[i] != ':'){
            number = number * 10 + (long)(s[i] - '0');
            i++;
        }// i point to ','
        if(s[i] == ':'){
            if(movierow != movierow1){
                for(int t = 0; t < customernumber; t++){
                    if(t != customernumber - 1){
                        fprintf(ofp, "%d\t", matrix[t]);
                    }else{
                        fprintf(ofp, "%d\n", matrix[t]);
                    }
                }
                free(matrix);
                matrix = (int*)malloc(customernumber*sizeof(int));
                if(matrix == NULL){
                    printf("not enough memory for matrix\n");
                    exit(0);
                }
                for(int t = 0; t < customernumber; t++){
                    matrix[t] = 0;
                }
            }// won't fprintf in first round
            movieId = (int)number;
            movieindex[movierow] = movieId;
            movierow++;
            continue;
        }else{
            if(number < CustNum){
                customerId = (int)number;
                if(CustomerNumber[customerId - 1] == -1){
                    CustomerNumber[customerId - 1] = realcustomerNum;
                    realcustomerNum++;
                }
                i++;
                rateNum ++;
                rate = (int)(s[i] - '0');
                if(rate > 5 || rate < 0){
                    rate = 0; //data clean
                    rateNum --;
                }
                matrix[customerId - 1] = rate;
            }
        }
        
    }
    //fprintf last movie's information;
    for(int t = 0; t < customernumber; t++){
        if(t != customernumber - 1){
            fprintf(ofp, "%d\t", matrix[t]);
        }else{
            fprintf(ofp, "%d\n", matrix[t]);
        }
    }
    fprintf(ofp2, "%d\n", movierow);
    fprintf(ofp2, "%d\n", realcustomerNum);
    fprintf(ofp2, "%ld\n", rateNum);
    *realCustNum = realcustomerNum;
    *realratenum = rateNum;
    free(matrix);
    fclose(ifp);
    fclose(ifp2);
    fclose(ofp);
    fclose(ofp2);
    printf("done!\n");
    twoarrays[0] = movieindex;
    twoarrays[1] = CustomerNumber;
    return movierow;
}

void compress(char* inputfile, char* outputfile, int* C, int CustNum){//不超过65535/2
    FILE* ifp;
    FILE * ofp;
    ifp = fopen(inputfile, "r");
    if(ifp == NULL){
        printf("inputfile does not exist\n");
        fclose(ifp);
        exit(1);
    }
    ofp = fopen(outputfile, "w");
    char s[65535];
    while((fgets(s,65535,ifp))!=NULL){
        for(int i = 0; i < CustNum; i++){
            if(C[i] != -1){
                fprintf(ofp, "%c\t", s[i*2]);
            }
        }
        fprintf(ofp, "\n");
    }
    fclose(ifp);
    fclose(ofp);
}
