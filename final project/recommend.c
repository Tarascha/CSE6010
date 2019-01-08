//
//  recommend.c
//  6140_project_2
//
//  Created by 陆垠全 on 12/1/18.
//  Copyright © 2018 Yinquan Lu. All rights reserved.
//

#include "recommend.h"
#include "dataIO.h"

// return point to predicted rate for each moive if succeed, or NULL if fail
double* predict(Parameters* parameters, int user_index) {
    // parameters check
    if(parameters == NULL || user_index < 0) {
        printf("invalid parameters\n");
        return NULL; // check!!!
    }
    
    int num_moive = parameters->num_moive;
    int lowRank = parameters->lowRank;
    double** U = parameters->U;
    double** V = parameters->V;
    
    double* prediction;
    if ((prediction = (double*) malloc (num_moive * sizeof(double))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
    
    // loop and do element-wise computing to get recommendation
    for(int i = 0; i < num_moive; i++) {
        double sum = 0;
        for(int j = 0; j < lowRank; j++) {
            sum += U[i][j] * V[user_index][j];
        }
        
        prediction[i] = sum;
    }
    
    return prediction;
}

// partition for quick sort
int partition(double* prediction, int low, int high, int* recommendation){
    // parameters check
    if(prediction == NULL || low < 0 || high < 0 || recommendation == NULL) {
        printf("invalid parameters\n");
        return -1;  // return -1 if fails
    }
    // record key value for partition
    double key;
    int key2;
    key = prediction[low];
    key2 = recommendation[low];
    
    // swtich postion based comparsion
    while(low<high){
        while(low < high && prediction[high] <= key ) {
            high--;
        }
        if(low<high){
            prediction[low] = prediction[high];
            recommendation[low] = recommendation[high];
            low ++;
        }
        
        while( low<high && prediction[low]>=key ){
            low++;
        }
        if(low<high){
            prediction[high] = prediction[low];
            recommendation[high] = recommendation[low];
            high --;
        }
    }
    prediction[low] = key;
    recommendation[low] = key2;
    return low;
}


// quick sort
void quick_sort(double* prediction, int start, int end, int* recommendation){
    int pos;
    if (start<end){
        // recursion
        pos = partition(prediction, start, end, recommendation);
        quick_sort(prediction,start,pos-1, recommendation);
        quick_sort(prediction,pos+1,end, recommendation);
    }
    return;
}

int* top_recommendation(Parameters* parameters, int user_index, int num_recommendation, Data* data_info) {
    // check parameter
    int num_moive =  data_info->num_movie;
    if(parameters == NULL || user_index < 0|| num_recommendation < 1 || data_info == NULL || num_recommendation > num_moive) {
        printf("invalid parameter\n");
        return NULL;  // check!!!
    }
    
    double* prediction = predict(parameters, user_index);
    double** rateMatrix = data_info->rateMatrix;
    int* recommendation;
    if ((recommendation = (int*) malloc (num_moive * sizeof(int))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
    int* top_recommendation;
    if ((top_recommendation = (int*) malloc (num_recommendation * sizeof(int))) == NULL) {fprintf(stderr, "malloc error\n"); exit(1);}
    
    
    // loop rate for certain user, fill recommendation with index value and mark rated moive in prediction
    for(int i = 0; i < num_moive; i++) {
        if(rateMatrix[i][user_index] != 0) {
            recommendation[i] = i;
            prediction[i] = -1;
            continue;
        }
        
        recommendation[i] = i;
    }
    

    //    double test[5] = {2.1, 4.1, 3.1, 8.1, 1.1};
    //    quick_sort(test, 0, 4, recommendation);
    //
    quick_sort(prediction, 0, num_moive-1, recommendation);
//    for(int i = 0; i < num_moive; i++) {
//        printf("prediction:%lf\t", prediction[i]);
//    }
//    printf("\n");
    for(int i = 0; i < num_recommendation; i++) {
        if(prediction[i] == -1) {
            
            printf("no enough recommended moives!\n");
            return NULL;  // check !!!
        }
        top_recommendation[i] = recommendation[i];
    }
    
    free(prediction);
    prediction = NULL;
    free(recommendation);
    recommendation = NULL;
    return top_recommendation;
    
}


void free_recommendation(int* top_recommendation) {
    // check parameters
    if(top_recommendation == NULL) {
        printf("invalid parameter\n");
        }
    
    free(top_recommendation);
}
