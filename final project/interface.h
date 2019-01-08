//
//  interface.h
//  6010finaltest2
//
//  Created by 刘昊 on 12/3/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#ifndef interface_h
#define interface_h
#include "recommend.h"
#include "matrix.h"
#include "matrix_factorization.h"
#include "dataIO.h"

Parameters* factorization(Data* data_info);
Data* load_data(const char* file_name, const char* file_name2);
int* top_recommendation(Parameters* parameters, int user_index, int num_recommendation, Data* data_info);

#endif /* interface_h */
