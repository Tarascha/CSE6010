//
//  main.c
//  assignment1C
//
//  Created by 刘昊 on 9/11/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#include <stdio.h>
#include "assignment1c.h"
    int main() {
        srand((unsigned)time(NULL));
        int data[20];
        double key[20];
        int i=0;
        for(i=0;i<20;i++){
            data[i]=i+1;
            key[i]=rand()/(double)(RAND_MAX/10);
            printf("%lf\t",key[i]);
            printf("%d\n",data[i]);
        }
        PrioQ*PQ=PQ_creat();
        for(i=0;i<20;i++){
            PQ_insert(PQ, key[i],&data[i]);
        }
      
        for(i=0;i<20;i++){
            int b=PQ_count(PQ);
            printf("remainning items:%d\n",b);
            int*a=PQ_delete(PQ,key);
            printf("deleted data:%d\t",*a);
            printf("key of this data:%lf\n",key[0]);
        }
        PQfree(PQ);
        return 0;
        
}
