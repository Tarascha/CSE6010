

#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include "interface.h"
#include "datapsetout.h"
void get_rank(int* C, int movienumber, int CUID, char* filename, Parameters* parameter, Data* data_info);

void viewallmoive(int moivenumber, char* filmname);
void giverateTomovie(int realcustnum, int moivenum, long realratenum, int CUINDEX, Data* data_info, Parameters* parameters, int lowRank, char* matrixfile, char* threefile);
void newUserRank(long realratenum, int CUID, Data* data_info, Parameters* parameters, int lowRank,  int movienumber, char* filmname, int realcustnum, int* C, char* matrixfile, char* threefile);
int logining(long realratenum, Data* data_info, Parameters* parameters, int lowRank, int movienumber, char* filmname, int realcustnum,int* C, char* matrixfile, char* threefile)
{
    int a=0,i=0;
    char uname[10],c=' ';
    char pword[10],code[10];
    //char user[10]="user";
    char pass[10]="pass";
    int CuID;
    do
    {
        
        printf("\n  ++++++++++++++++++++++++++++++  LOGIN FIRST  ++++++++++++++++++++++++++++++  ");
        printf(" \n\n                  ENTER USERID:-");
        scanf("%s", &uname);
        sscanf(uname,"%d",&CuID);
        printf(" \n\n                  ENTER PASSWORD:-");
        //getch(); // inorder to hold the screen untill user press something
        // enter password
        scanf("%s",&pword);
       
        //char code=pword;
        i=0;
        //scanf("%s",&pword);
        if(strcmp(pword,"pass")==0)
        {   

            if(CuID > 29999 || CuID < 1){
                printf("userID should between 1 to 29999, you are not a valid customer, sorry\n");
                exit(1);
            }
            if(C[CuID - 1] != -1){
                printf("  \n\n\n       WELCOME TO OUR Movie SYSTEM !!!! LOGIN IS SUCCESSFUL");
                printf("\n\n\n\t\t\t\tJust enter anything you want and then press Enter to Continue...\n");
                char presskey[20];
                scanf("%s", &presskey);//holds the screen
                break;
            }else{
                printf("hello new customer!!!!!!!!\n");
                //                newUserRank(realratenum, CuID, data_info, parameters, lowRank, movienumber, filmname, realcustnum, C, matrixfile, threefile);
                //                data_info = load_data(matrixfile, threefile);
                //                parameters = factorization(data_info, lowRank);
                //                save_parameters(parameters, outputUV);
                break;
            }
            
        }
        else
        {
            printf("\n        SORRY !!!!  LOGIN IS UNSUCESSFUL\n");
            a++;
            
            //getch();//holds the screen
            
        }
    }
    while(a<=2);
    if (a>2)
    {
        printf("\nSorry you have entered the wrong username and password for four times!!!\n");
        exit(1);
        //getch();
        
    }
    //system("cls");
    return CuID;
}


//


//
int main(int argc, const char * argv[])
{   char* inputfile = argv[1];
    char* inputfile2 = argv[2];
    char* filmname = argv[3];
    
//    char* inputfile ="/Users/liuhao/Desktop/CSE 6010/final project/6010final/testdata.txt";
//    char* inputfile2 = "/Users/liuhao/Desktop/CSE 6010/final project/6010final/testdata2.txt";
//    char* filmname = "/Users/liuhao/Desktop/CSE 6010/final project/6010final/moviesimplename.txt";
    char* outputfile = argv[4];
    char* outputfile2 = argv[5];
    char* outputfile3 = argv[6];
    int** twoarrays = (int**)malloc(2*sizeof(int*));
    int realcustnum;
    long realratenum;
    int moivenumber = readindata(inputfile, inputfile2, outputfile,outputfile2, twoarrays, &realcustnum, &realratenum, 17770, 30000);//right most parameter of these two functions must be the same//17770, 30000
    int* movieindex = twoarrays[0];
    int* CustomerisrealOrNot = twoarrays[1];
    compress(outputfile, outputfile3, CustomerisrealOrNot, 30000);
    //    printf("%d\n", moivenumber);
    //    printf("%d\n", realcustnum);
    //    printf("%ld\n", realratenum);
    //=================================================
    int seed = (unsigned int) time(NULL);
    srand(seed);
    
    // delecare variable
    Data* data_info;
    Parameters* parameters;
    int num_moive;
    int num_user;
    int num_rate;
    int lowRank;
    
    // read in data
    data_info = load_data(outputfile3, outputfile2);
    num_moive = data_info->num_movie;
    num_user = data_info->num_user;
    num_rate = data_info->num_rate;
    lowRank = 3;
    printf("number of movie: %d\n", data_info->num_movie);
    // training model
    parameters = factorization(data_info);
    printf("number movieeeeee:%d\n", parameters->num_moive);
    
    
    //================================================
    
    int CUID = logining(realratenum, data_info, parameters, lowRank, moivenumber, filmname, realcustnum, CustomerisrealOrNot, outputfile3, outputfile2);
    
    if(CustomerisrealOrNot[CUID -1] == -1){// get user's ID // if this user is a new customer
        
        newUserRank(realratenum, CUID, data_info, parameters, lowRank, moivenumber, filmname, realcustnum, CustomerisrealOrNot, outputfile3, outputfile2);
        realcustnum++;
        data_info = load_data(outputfile3, outputfile2);
        parameters = factorization(data_info);
       //retrain model
    }
    int CUINDEX = CustomerisrealOrNot[CUID - 1];// get user's index in training matrix
    int ch;
    do{
        printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        printf("\n");
        printf("\t Movie Recommendation System");
        printf("\n");
        printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        
        printf("\nEnter >1< To get your favourite movie");
        printf("\nEnter >2< To View All Movie");
        printf("\nEnter >3< To give rate to a Movie ");
        printf("\nEnter >0< To Exit ");
        
        printf("\nEnter your Choice ::");
        scanf("%d",&ch);
        
        
        switch (ch)//three choice for customer
        {
            case 1 :
                get_rank(CustomerisrealOrNot, moivenumber, CUID, filmname, parameters, data_info);
                break;
                
            case 2:
                viewallmoive(moivenumber, filmname);
                break;
                
            case 3:
                
                giverateTomovie(realcustnum, moivenumber, realratenum, CUINDEX, data_info, parameters, lowRank, outputfile3, outputfile2);//
                data_info = load_data(outputfile3, outputfile2);///you wen ti ?
                parameters = factorization(data_info);
                //save_parameters(parameters, outputUV);
                printf("retrained modole~~~~~~\n");
                break;

            case 0:

                printf("Thank you for your use , we expect you next time!\n");
                break;
                
            default:
                printf("Wrong choice !");
                exit(1);
                break;
        }
    }while(ch!=0);
    //
    free_Data(data_info);
    data_info = NULL;
    free_parameters(parameters);
    parameters = NULL;
    free(twoarrays[0]);
    free(twoarrays[1]);
    free(twoarrays);
    return 0;
    
    
}


//    printf("Please enter your Customer ID:-");
//    long CustomerID;
//    scanf("%ld", &CustomerID);
//    printf("looking for moives.....................");



//this function is to print out top 10 movies to customer
void get_rank(int* C, int movienumber, int CUID, char* filmname, Parameters* parameter, Data* data_info){
    char presskey[20];
    printf("Welcome Old Friend\n");
    printf("looking for moives.....................\n");
    int* recommendarray;
    int CUSTINDEX = C[CUID - 1];
    //get CustomerIndex into Interface--------------------------------------------
    recommendarray = top_recommendation(parameter, CUSTINDEX, 10, data_info);// 2 = number of recmovie
    //get a double array out-------------------------------------------
    if(recommendarray == NULL){
        return;
    }
    for(int i = 0; i < 10; i++){
        FILE* ifp;
        ifp = fopen(filmname, "r");
        if(ifp == NULL){
            printf("load movietxt fail\n");
            exit(1);
        }
        int movieID = recommendarray[i] + 1;
        //printf("\n\n\nrecommened movieID : %d\n\n\n", movieID);
        
        printf("%d", movieID);
        char s[1000];
        for(int j = 0; j < movieID; j++){
            fgets(s,1000,ifp);
        }
        int t = 0;
        while(s[t] != ','){
            t++;
        }
        t++;
        while(s[t] != ','){
            t++;
        }
        for(t; s[t] != '\n'; t++){
            printf("%c", s[t]);
        }
        printf("\n");
        fclose(ifp);
    }
    free_recommendation(recommendarray);
    printf("  \n\n These are the movies we recommened!");
    printf("\n\n\n\t\t\t\tJust enter anything you want and then press Enter back to main menu...");
    //hold the screen
    scanf("%s", &presskey);
    
}

// this function is to print out all the movies system have for customers
void viewallmoive(int moivenumber, char* filmname){
    char presskey[20];
    FILE * ifp;
    ifp = fopen(filmname, "r");
    if(ifp == NULL){
        printf("cannot load movie information\n");
        exit(1);
    }
    char s[100];
    int k = 0;
    while(fgets(s, 100, ifp) != NULL && k < moivenumber){
        k++;
        printf("%d",k);
        int t = 0;
        while(s[t] != ','){
            t++;
        }
        t++;
        while(s[t] != ','){
            t++;
        }
        for(t; s[t] != '\n'; t++){
            printf("%c", s[t]);
        }
        printf("\n");
    }
    printf("  \n\n That's all the movies we have!");
    printf("\n\n\n\t\t\t\tJust enter anything you want and then press Enter go to main menu...");
    scanf("%s", &presskey);
    
    
}

//this function is to alter the data when customer change their ratings to movies
void giverateTomovie(int realcustnum, int moivenum, long realratenum, int CUINDEX, Data* data_info, Parameters* parameters, int lowRank, char* matrixfile, char* threefile){
    
    int ch;
    printf("Enter 1 to rate a movie\n Enter any other button to go back to main menu\n");
    scanf("%d", &ch);
    if(ch == 1){
        do{
            printf("Hello dear customr, please enter the movie ID you want to give a rate:\n");
            int movieid;
            scanf("%d", &movieid);
            if(movieid > 9210 || movieid < 1){
                printf("sorry, movieId should between 1 to 9210, please enter another movieId\n");
                continue;
            }
            printf("How much do you like this movie? please enter(1~5):\n");
            int rate;
            scanf("%d", & rate);
            if(rate >= 1 && rate <= 5){
                realratenum++;
                data_info->rateMatrix[movieid - 1][CUINDEX] = (double)rate;
                printf("rated successfully, press 1 to continue rate a movie, press any other button to go back to main menu\n");
            }else{
                printf("not a valid rate, press 1 to continue rate a movie, any other button to go back to main menu\n");
            }
            
            scanf("%d", &ch);
        }while(ch == 1);
        FILE* ofp1;
        FILE* ofp2;
        ofp1 = fopen(matrixfile, "w");
        ofp2 = fopen(threefile, "w");
        if(ofp1 == NULL || ofp2 == NULL){
            printf("can't open file\n");
            exit(1);
        }
        fprintf(ofp2, "%d\n", moivenum);// re fprint out parameters
        fprintf(ofp2, "%d\n", realcustnum);
        fprintf(ofp2, "%ld\n", realratenum);
        
        for(int i = 0; i < moivenum; i++){// re fprint out training matrix
            for(int j = 0; j < realcustnum; j++){
                fprintf(ofp1, "%d\t",(int)data_info->rateMatrix[i][j]);
            }
            fprintf(ofp1, "\n");
            
        }
       
        fclose(ofp1);
        fclose(ofp2);
        
    }else{
        printf("going back to main menu..........................!\n");
        return;
    }
    
    
    return;
    
    
}

//this function is for new users to give rates to movies when log into system
void newUserRank(long realratenum, int CUID, Data* data_info, Parameters* parameters, int lowRank, int movienumber, char* filmname, int realcustnum, int* C, char* matrixfile, char* threefile){
    C[CUID - 1] = realcustnum;
    int CUINDEX = realcustnum;
    for(int i = 0; i < movienumber; i++){
        double* newrow = (double*) realloc(data_info->rateMatrix[i], (realcustnum + 1)*sizeof(double));
        if(newrow != NULL){//change the size of training data , since this new customer should be added
            data_info->rateMatrix[i] = newrow;
        }else{
            printf("realloc failed!\n");
        }
        data_info->rateMatrix[i][realcustnum] = 0;
    }
    
    printf("Since you are new Customer, please give us your rating to some movies\n");
    printf("you can view all movies by press 2, or just press 1 to rate movies\n");
    int ch;
    scanf("%d", &ch);
    if(ch == 2){
        viewallmoive(movienumber, filmname);
        printf("\n\nyou have viewd movies we have, now please give us your rate to some movie\n\n");
        do{
            printf("Hello dear customr, please enter the movie ID you want to give a rate:\n");
            int movieid;
            scanf("%d", &movieid);
            if(movieid > 9210 || movieid < 1){
                printf("sorry, movieId should between 1 to 9210, please enter another movieId\n");
                ch = 1;
                continue;
            }
            printf("How much do you like this movie? please enter(1~5):\n");
            int rate;
            scanf("%d", & rate);
            if(rate >= 1 && rate <= 5){
                realratenum++;
                data_info->rateMatrix[movieid - 1][CUINDEX] = rate;
                printf("rated successfully, press 1 to continue rate a movie, press any other button back to main menu\n");
            }else{
                printf("not a valid rate, press 1 to continue rate a movie, press any other button back to main menu\n");
            }
            scanf("%d", &ch);
        }while(ch == 1);
        FILE* ofp1;
        FILE* ofp2;
        ofp1 = fopen(matrixfile, "w");
        ofp2 = fopen(threefile, "w");
        if(ofp1 == NULL && ofp2 == NULL){
            printf("can't open file\n");
            exit(1);
        }
        realcustnum++;
        fprintf(ofp2, "%d\n", movienumber);
        fprintf(ofp2, "%d\n", realcustnum);
        fprintf(ofp2, "%ld\n", realratenum);
        for(int i = 0; i < movienumber; i++){
            for(int j = 0; j < realcustnum; j++){
                fprintf(ofp1, "%d\t",(int)data_info->rateMatrix[i][j]);
            }
            fprintf(ofp1, "\n");
        }
        fclose(ofp1);
        fclose(ofp2);
        //re train;
        //        data_info = load_data(matrixfile, threefile);
        //        parameters = factorization(data_info, lowRank);
        //        save_parameters(parameters, outputUV);
        
        
    }else if(ch == 1){
        do{
            printf("\nHello dear customr, please enter the movie ID you want to give a rate:\n");
            int movieid;
            scanf("%d", &movieid);
            if(movieid > 9210 || movieid < 1){
                printf("sorry, movieId should between 1 to 9210, please enter another movieId\n");
                continue;
            }
            printf("How much do you like this movie? please enter(1~5):\n");
            int rate;
            scanf("%d", & rate);
            if(rate >= 1 && rate <= 5){
                realratenum++;
                data_info->rateMatrix[movieid - 1][CUINDEX] = rate;
                printf("rated successfully, press 1 to continue rate a movie, press any other button back to main menu\n");
            }else{
                printf("not a valid rate, press 1 to continue rate a movie, press any other button back to main menu\n");
            }
            scanf("%d", &ch);
        }while(ch == 1);
        FILE* ofp1;
        FILE* ofp2;
        ofp1 = fopen(matrixfile, "w");
        ofp2 = fopen(threefile, "w");
        if(ofp1 == NULL && ofp2 == NULL){
            printf("can't open file\n");
            exit(1);
        }
        realcustnum++;
        fprintf(ofp2, "%d\n", movienumber);
        fprintf(ofp2, "%d\n", realcustnum);
        fprintf(ofp2, "%ld\n", realratenum);
        for(int i = 0; i < movienumber; i++){
            for(int j = 0; j < realcustnum; j++){
                fprintf(ofp1, "%d\t",(int)data_info->rateMatrix[i][j]);
            }
            fprintf(ofp1, "\n");
        }
       
        fclose(ofp1);
        fclose(ofp2);
        
    }else{
        printf("Sorry! wrong button! since you are a new customer, you must first give us some ratings before you can get recommend movie !\n");
        do{
            printf("\nHello dear customr, please enter the movie ID you want to give a rate:\n");
            int movieid;
            scanf("%d", &movieid);
            if(movieid > 9210 || movieid < 1){
                printf("sorry, movieId should between 1 to 9210, please enter another movieId\n");
                continue;
            }
            printf("How much do you like this movie? please enter(1~5):\n");
            int rate;
            scanf("%d", & rate);
            if(rate >= 1 && rate <= 5){
                realratenum++;
                data_info->rateMatrix[movieid - 1][CUINDEX] = rate;
                printf("rated successfully, press 1 to continue rate a movie, press any other button back to main menu\n");
            }else{
                printf("not a valid rate, press 1 to continue rate a movie, press any other button back to main menu\n");
            }
            scanf("%d", &ch);
        }while(ch == 1);
        FILE* ofp1;
        FILE* ofp2;
        ofp1 = fopen(matrixfile, "w");
        ofp2 = fopen(threefile, "w");
        if(ofp1 == NULL && ofp2 == NULL){
            printf("can't open file\n");
            exit(1);
        }
        realcustnum++;
        fprintf(ofp2, "%d\n", movienumber);
        fprintf(ofp2, "%d\n", realcustnum);
        fprintf(ofp2, "%ld\n", realratenum);
        for(int i = 0; i < movienumber; i++){
            for(int j = 0; j < realcustnum; j++){
                fprintf(ofp1, "%d\t",(int)data_info->rateMatrix[i][j]);
            }
            fprintf(ofp1, "\n");
        }
       
        fclose(ofp1);
        fclose(ofp2);
    }
    
}
