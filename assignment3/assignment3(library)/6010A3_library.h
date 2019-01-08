//
//  6010A3_library.h
//  6010A3
//
//  Created by 刘昊 on 10/9/18.
//  Copyright © 2018 刘昊. All rights reserved.
//

#ifndef _010A3_library_h
#define _010A3_library_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
//function of engine----------------------------------------------------
void Schedule(double ts, void* data);
double CurrentTime (void);
void PrintList (void);
void RunSim (double EndTime);
//function of application------------------------------------------------
// functions to initialize components
void MakeGenerator (int GenID, double IntTime, int DestID);
//void MakeQS(int QSID, double SerTime, int FromID, int DestID);
void MakeQS(int QSID, double SerTime, int DestID);
//void MakeFork(int ForkID, int totalnumber, int FromID, int ID[], double possibility[] );
void MakeFork(int ForkID, int totalnumber, int ID[], double possibility[] );
void MakeExit (int ExitID);
//functions to get state variables
int getenteredCustomers(void);
int getexitedCustomers(void);
double getminimumtime(void);
double getmaximumtime(void);
double getavgtime(void);
double getminimumtimeinQS(void);
double getmaximumtimeinQS(void);
double getavgtimeinQS(void);
double avgtimeofQS(int CompID);
void EventHandler (void *data);
#endif /* _010A3_library_h */
