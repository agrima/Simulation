//test the Chi Square Test

#include <stdio.h>
#include <math.h>

#define SIZE 400 //generate 400 random variate

#define NUM 10 // the number of interval is 10

#define A 0 // the value range is from 0 to 1

#define B 1

double uniform(double *);
double expon(double *, float);

int main() {

  int i,j,l,sum;
  double chisquare,E[NUM];
  double seed;
  double r;
  double O[NUM];
  double a,b,num,interval;

  //set initial values
  for(i=0;i<NUM;i++){E[i]=SIZE/NUM;}
  a=A;b=B;num=NUM;sum=0;
  interval=(b-a)/num;
  seed = 1234567; //set the seed only once, at the start.
  for (j=0;j<NUM;j++){O[j]=0;}
  //end initial values

  for (i=0; i < SIZE; i++) {
	  r = uniform(&seed); //generate an uniform variate
	  for (j=0;j<NUM;j++){
		  if ((a+interval*j)<=r&&r<(a+interval*(j+1))){ //judge which interval it belongs to
			  O[j]++;}
		int k;
		for(k=0;k<NUM;k++){

			sum+=pow((O[k]-E[k]),2)/E[k];
			chisquare=sum; //compute the chisquare test

		}
	  }		  
	  
  }
  //Testing the Null Hypothesis
  if(chisquare>16.919)
  	{printf("Reject H0");

  		}
  else{
  		printf("Fail to Reject H0");

  }

}
