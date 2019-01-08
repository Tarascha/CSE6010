//
//  head26010.c
//  assignment26010
//
//  Created by 刘昊 on 9/22/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#include "head26010.h"
int** generate_martix(int k, float p){
    if( k <= 0 || p < 0){
        printf("error requirement!\n");
        return NULL;
    }
        
    int** gpointer = (int**)malloc(k*sizeof(int*));
    if(gpointer == NULL){
        printf("error,lacking memory");
        return 0;
    }
    int i = 0, j = 0,dice = 0, count = 0;
    for(i = 0; i < k; i++){
        gpointer[i] = (int*)malloc(k*sizeof(int));
        if(gpointer[i] == NULL){
            printf("error,lacking of memory");
            return 0;
        }
    }
    srand((unsigned)time(NULL));
    for(i = 0; i < k; i++){
        for(j = 0; j < k; j++){
            dice = rand() % 100;
            if(dice <= 100 * p){
                gpointer[i][j] = count;
                count ++;
            }else{
                gpointer[i][j] = -1;
            }
        }
    }
    return gpointer;
}

void free_matrix(int k,int** a){
    int i=0;
    for(i=0;i<k;i++)
        free(a[i]);
    free(a);
    
}

void print_matrix(int** gpointer, int k){
    printf("generated matrix is:\n");
    for(int i = 0; i < k; i++){
        for(int j = 0; j < k; j++){
            printf("%d\t",gpointer[i][j]);
        }
        printf("\n");
    }
    
}

void writein(char* outputfire, int** gopinter, int k){
    FILE* ofp;
    int i, j,count = 0;
    ofp = fopen(outputfire, "w");
    if(ofp == NULL){
        printf("can't open file\n");
        fclose(ofp);
               exit(0);
    }
    
    
    
    for(i = 0; i < k; i++){ // count how many nodes in the graph
        for ( j = 0; j < k; j++){
            if (gopinter[i][j] != -1){
            count ++;
            }
         }
    }
    
    int position[count][3];// record the position of those nodes;
    int t = 0;
    for(i = 0; i < k ; i++){
        for(j = 0; j < k; j++ ){
            if(gopinter[i][j] != -1){
                position[t][0] = i;
                position[t][1] = j;
                position[t][2] = gopinter[i][j];
                t++;
            }
        }
    }// recorded
    
    //print position first to test
    printf("position of each nodes:\n");
    for(i = 0; i < count; i++){
        for( j = 0; j < 3; j++){
            printf("%d\t",position[i][j]);
        }
        printf("\n");
    }//all right before;
    
    //char str[count + 1][256];//str is used to input into file
    int pcount = count;
//    while(pcount != 0){// put count into first row of str
//        i = 0;
//        int tmp = pcount % 10;// when nodenumber is beyond 9, a char cannot hold that!
//        str[0][i] = tmp + '0';
//        i++;
//        pcount = pcount / 10;
//    }
//    str[0][i] = '\n';
    fprintf(ofp, "%d\n",pcount);
    
    for(i = 1; i <= count; i++){// now starts to put nodes and it's neighbour into str;
        //int r = 0 ;
        int nodenumber = position[i-1][2];
//        if( nodenumber == 0 ){
//            str[i][r] = '0';
//            r++;
//        }
//        while(nodenumber != 0){
//        str[i][r] = nodenumber % 10 +'0'; // put node number into first column of str;
//            r++;
//            nodenumber = nodenumber /10;
//        }
//        str[i][r] = '\t';
//        r++;
        fprintf(ofp, "%d\t",nodenumber);
        for(j = 0; j < count; j++){// if any other nodes' position is next to this node, put it in ;
            if((position[j][0] == position[i-1][0] && abs(position[j][1] - position[i-1][1]) == 1) || (position[j][1] == position[i-1][1] && abs(position[j][0] - position[i-1][0]) == 1)){
                nodenumber = position[j][2];
//                if(nodenumber == 0){
//                    str[i][r] = '0';
//                    r++;
//                }
//                while(nodenumber != 0){
//                    str[i][r] = nodenumber % 10 + '0';
//                    r++;
//                    nodenumber = nodenumber / 10;
//                }
//                str[i][r] = '\t';
//                r++;
                fprintf(ofp, "%d\t",nodenumber);
            }
        }
//        str[i][r] = '\n';
        fprintf(ofp, "\n");
        
    }//done, str has been builded!
    fclose(ofp);
    
    
    
//    for(i = 0; i <= count; i++){//write str into the file;
//        fputs(str[i],ofp);
//        fputs("\n",ofp);
//    }
}
