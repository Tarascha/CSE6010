//
//  head26010.h
//  assignment26010
//
//  Created by 刘昊 on 9/22/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#ifndef head26010_h
#define head26010_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int** generate_martix(int k, float p);
void free_matrix(int k,int** a);
void print_matrix(int** gpointer, int k);
void writein(char* outputfire, int** gopinter, int k);

#endif /* head26010_h */
