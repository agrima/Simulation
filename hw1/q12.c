//test the exponential random number generator
#include <stdio.h>

#define SIZE 2000 //generate 2000 random variate

#define NUM 100 // the number of interval is 100

#define A 0 // the value range is from 0 to 100

#define B 100

#define EXPMEAN 10 //the expected mean is 10

double uniform(double *);
double expon(double *, float);

int main() {

  int i,j;
  float emean = EXPMEAN;
  double mean,sum;
  double seed;
  double r;
  double h[NUM];
  double a,b,num,interval;

 //initialization
  sum=0;
  a=A;b=B;num=NUM;
  interval=(b-a)/num;
  seed = 1234567;
  for (j=0;j<NUM;j++){h[j]=0;}
  

  for (i=0; i < SIZE; i++) {
      r = expon(&seed,emean); //generate an exponential variate
	  for (j=0;j<NUM;j++){
		  if ((a+interval*j)<=r&&r<(a+interval*(j+1))){ //judge which interval it belongs to
			  h[j]++;}
	  }		  
	  sum=sum+r; 
  }
  mean=sum/SIZE; //compute the mean

  printf("\n The mean is %g",mean); //print the mean

}
