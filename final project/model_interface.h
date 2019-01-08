//
//  interface.h
//  6010finaltest2
//
//  Created by Yinquan Lu and Hao Liu on 12/3/18.
//  Copyright © 2018 Hao Liu. All rights reserved.
//

#ifndef interface_h
#define interface_h
#include "recommend.h"
#include "matrix.h"
#include "matrix_factorization.h"
#include "dataIO.h"

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

/*
 * low rank matrix factorization
 *
 * @param data_info: trained data
 * @return a point to model parameters if successful, and NULL on error.
 */
Parameters* factorization(Data* data_info);


/*
 * free parameters
 *
 * @param parameters: memory waited to be free
 * @return NULL on error.
 */
void free_parameters(Parameters* parameters);

/*
 * give recommendation
 *
 * @param parameters: model
 * @param user_index: user index
 * @param num_recommendation: the number of recommendation moive
 * @param data_info: rate data
 * @return a point to a array including recommendation results if successful, and NULL on error.
 */
int* top_recommendation(Parameters* parameters, int user_index, int num_recommendation, Data* data_info);


/*
 * free recommendation
 *
 * @param top_recommendation: a point to recommendation result
 */
void free_recommendation(int* top_recommendation);


#endif /* interface_h */
