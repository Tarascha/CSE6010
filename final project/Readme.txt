Author: Yinquan Lu
		Hao Liu
Created: November 19, 2018
Last Modified: December 1, 2018
Affiliation:Georgia Institue of Technology

Description
--------------
This program is a Movie recommendation system, based on the data set
and customers' ratings, this system can recommend movies customers
may like.

After compiled and ran the program, please wait for a few minutes
(basically 2~3 minutes)before you can see the user interface, since
program need time to prepare traning model.

You can use UserID,which is a number from 1 to 29999 and the
Password which is "pass" to log into the system.Some IDs are
old customers(which means the data set already has these customers'
ratings toward some movies) and other's are new customers (which means dataset do not contain these customers' rating)
For example, ID 2442 is an old customer and ID 356 is a new customer
If a new customer log into this system, he or she must first give
some ratings towards some movies before he or she can have a view
of recommended movies.

After succesfully logged into this system, there will be three
options, first is to get the name of recommended movies, second is to
view all the movie this system has(9210 movies), third is to give rates(1~5) to the movies you want.
If a customer changed the rates he or she give to movies, the result 
of recommendation will change too.

Installation
---------------
To install, simply run

gcc -std=c99 main.c dataIO.c datapsetout.c matrix.c matrix_factorization.c recommend.c -o test -lm

Execution
---------------
Assuming your executable is called "test", run it using
./test combined_data_1.txt combined_data_2.txt moviename.txt outputdata.txt outputdata2.txt outputdata3.txt
