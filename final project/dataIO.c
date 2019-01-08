//
//  dataIO.c
//  6140_project_2
//
//  Created by Yinquan Lu on 11/30/18.
//  Copyright © 2018 Yinquan Lu. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dataIO.h"
#include "matrix.h"


Data* load_data(const char* file_name, const char* file_name2) {
    // parameter check
    if(file_name == NULL || file_name2 == NULL) {
        printf("invalid file route\n");
        return NULL;  // check!!!!
    }
    
    // record run time for load data
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    // open input file
    printf("loading data...\n");
    FILE* ifp;
    ifp = fopen(file_name, "r");
    if(ifp == NULL) {
        printf("inputfile io error\n");
        return NULL;  // operation fails: return 0
    }
    
    FILE* ifp2;
    ifp2 = fopen(file_name2, "r");
    if(ifp2 == NULL) {
        printf("inputfile io error\n");
        return NULL;  // operation fails: return 0
    }
    
    // read in num of moive, num of user, num of rate
    char* line2 = (char*)  malloc(100*sizeof(char));
    // Check if allocation was successful by checking if malloc returned NULL
    if (line2==NULL) {
        printf("\nMemory allocation failed. Abort.\n");
        return 0; // operation fails: return 0
    }
    fgets(line2, 100, ifp2);
    int n1 = atoi(line2);  // num of moive
    fgets(line2, 100, ifp2);
    int n2 = atoi(line2);  // num of user
    fgets(line2, 100, ifp2);
    int n3 = atoi(line2);  // num of rate

    // initialize and malloc
    Data* data_info;
    double** mat;
    rate_node* rate_list;
    
    if ((data_info = (Data*) malloc (sizeof(Data))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
    mat = malloc_matrix(n1, n2);
    if ((rate_list = (rate_node*) malloc (n3 * sizeof(rate_node))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
    
    int max_memory = 2*n2 + 2;
    char* line = (char*)  malloc(max_memory*sizeof(char));
    // Check if allocation was successful by checking if malloc returned NULL
    if (line==NULL) {
        printf("\nMemory allocation failed. Abort.\n");
        return 0; // operation fails: return 0
    }
    
    int line_num = 0; // record current line index
    int rate_index = 0; // record index in rate_list
    // traverse each line
    while(fgets(line, max_memory, ifp)) {
//        printf("%s\n", line);
        // loop each cell and fill matrix
        for(int i=0; i<2*n2; i += 2) {
            char c = line[i];
            double r = c - '0';
            mat[line_num][i/2] = r;
            
            // check invalid rate
            if(r != 0 && r!=1 && r!=2 && r!=3 && r!=4 && r!=5) {
                printf("invalid rate\n");
                return NULL;  // check!!!
            }
            
            if(r > 0) {
                rate_node node;
                node.movie = line_num;
                node.user = i/2;
                node.rate = r;
                node.err = 0.0;
                rate_list[rate_index] = node;
                rate_index++;
            }
        }
        line_num ++;
    }
    
    
    data_info->num_movie = n1;
    data_info->num_user = n2;
    data_info->num_rate = n3;
    data_info->rateMatrix = mat;
    data_info->rate_list = rate_list;
    
    free(line);
    line = NULL;
    fclose(ifp);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time for load data: %lf\n", cpu_time_used);
    
    return data_info;
    
}


void free_Data(Data* data_info) {
    // check parameters
    if(data_info == NULL) {
        printf("invalid parameter\n");
    }
    
    int num_moive = data_info->num_movie;
    int num_user = data_info->num_user;
    free(data_info->rate_list);
    data_info->rate_list = NULL;
    free_matrix(num_moive, num_user, data_info->rateMatrix);
    data_info->rateMatrix = NULL;
    free(data_info);  // free Data, matrix, line, rate_list!!!
}
