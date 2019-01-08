//
//  matrix_factorization.h
//  6140_project_2
//
//  Created by Yinquan Lu on 11/30/18.
//  Copyright © 2018 Yinquan Lu. All rights reserved.
//

#ifndef matrix_factorization_h
#define matrix_factorization_h

#include <stdio.h>
#include <stdlib.h>
#include "dataIO.h"

// data struct for store parameters of model
typedef struct Parameters {
    double** U;
    double** V;
    int lowRank;
    int num_moive;
    int num_user;
} Parameters;


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

#endif /* matrix_factorization_h */
