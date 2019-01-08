//
//  main.c
//  assignment26010
//
//  Created by 刘昊 on 9/22/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#include <stdio.h>
#include "head26010.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    int size = 45;
    float possibility = 0.9;
    
    int **a = generate_martix(size, possibility);
    print_matrix(a, size);
    char* writefile = "/Users/liuhao/Desktop/CSE 6010/assignment2/assignment26010/writein.txt";
    writein(writefile, a, size);
    free_matrix(size, a);
    return 0;
    
}

