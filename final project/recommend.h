//
//  recommend.h
//  6140_project_2
//
//  Created by 陆垠全 on 12/1/18.
//  Copyright © 2018 Yinquan Lu. All rights reserved.
//

#ifndef recommend_h
#define recommend_h

#include <stdio.h>
#include "matrix_factorization.h"

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

#endif /* recommend_h */
