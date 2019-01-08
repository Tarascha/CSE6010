//
//  main.c
//  assignment2_16010
//
//  Created by 刘昊 on 9/23/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#include <stdio.h>
#include "assignment2_26010.h"

int main(int argc, const char * argv[]) {
     char* inputfile = "/Users/liuhao/Desktop/CSE 6010/assignment2/assignment26010/writein.txt" ;
     char* outputfile = "/Users/liuhao/Desktop/CSE 6010/assignment2/assignment26010/readwith.txt";
    int cc = 0;
    nodes** a = readinfile(inputfile, &cc);//a is original pointer of adjuncy list
    printf("number of nodes:%d\n",cc);
    printlinkedlist(a,cc);
    
  
    int color[cc + 1];
    for(int i = 0 ;i < cc + 1; i++){
        color[i] = 0;
    }
    analyse(outputfile,a, cc, color);
    //travel(a, 0, color);
    
    freelinkedlist(a,cc);
    return 0;
}
