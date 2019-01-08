//
//  dataIO.h
//  6140_project_2
//
//  Created by Yinquan Lu on 11/30/18.
//  Copyright © 2018 Yinquan Lu. All rights reserved.
//

#ifndef dataIO_h
#define dataIO_h

#include <stdio.h>

// data struct for input data
typedef struct rate_node {
    int rate;
    int movie;
    int user;
    double err;
} rate_node;
typedef struct Data {
    int num_movie;
    int num_user;
    int num_rate;
    double** rateMatrix;
    rate_node* rate_list;
} Data;


/*
 * load data for rate
 *
 * @param file_name: input file route for rate matrix
 * @param file_name2: input file route for rate statistic
 * @return a point to a data struct Data if successful, and NULL on error.
 */
Data* load_data(const char* file_name, const char* file_name2);


/*
 * free allocated memory for Data
 *
 * @param data_info: Data waited to free
 * @return NULL if fail
 */
void free_Data(Data* data_info);


#endif /* dataIO_h */
