#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double expon(double *, float);

#define TOTALJOBS 5		//The number of customers per run
#define EXPARRI 10		//The expected customer interarrival time
#define EXPSERVICE 20		//The expected service time
#define INFINITE 100000000	//Indicate a very large value
#define IDLE 0
#define BUSY 1
#define MAXQ 1000			//Number of queues in a tandem queue

double seed = 7654321;
double arr[TOTALJOBS];

double runtime[MAXQ];			//record the runtime for each N

int i[MAXQ];				//current customer being served
int j[MAXQ];				//next customer to arr
double servstatus[MAXQ];	//status of each server
double nextarr[MAXQ];	//next arrival event for each server
double nextdep[MAXQ];	//next depure event for each server
int qlength[MAXQ];		//queue length of each server
int finish;			//record #jobs that have finished all services

int isarr;			//earlest event is arr or not
int earliestqueue;		//in which queue the earliest event occurs
double earliesttime;		//the time of the earliest event

//Time of arrival to the first queue of each job
void init(void)
{
	int k;
	arr[0] = expon(&seed,EXPARRI);
	for(k=1; k<TOTALJOBS; k++)
	{
		arr[k] = arr[k-1] + expon(&seed,EXPARRI);
	}
}

void dep_handler(int n, double time, int N)
{
	i[n] ++;
	if (qlength[n] != 0)
	{
		qlength[n] --;
		servstatus[n] = BUSY;
		nextdep[n] = time + expon(&seed,EXPSERVICE);
		
	}
	else
	{
		servstatus[n] = IDLE;
		nextdep[n] = INFINITE;
	}
	if(n+1 < N)
		nextarr[n+1] = time + 0.1;
	
}


void arr_handler(int n, double time)
{
	time = time;
	j[n] ++;
	if(servstatus[n] == IDLE)
	{
		servstatus[n] = BUSY;
		nextdep[n] = time + expon(&seed,EXPSERVICE);
	}
	else
	{
		qlength[n] = qlength[n] + 1;
	}
	if((n == 0) && (j[n] < TOTALJOBS))
		nextarr[n] = arr[j[n]];
	else if ((n != 0) && (j[n] < TOTALJOBS))
		nextarr[n] = nextdep[n-1] + 0.1;
	else
		nextarr[n] = INFINITE;
}

void earliest(int N)
{
	double min;
	int e;
	earliestqueue = 0;
	min = nextarr[earliestqueue];	
	isarr = 1;
	for(e = 0; e < N; e ++)
	{
		if(nextarr[e] < min)
		{
			min = nextarr[e];
			isarr = 1;
			earliestqueue = e;
		}
		if(nextdep[e] < min)
		{
			min = nextdep[e];
			isarr = 0;
			earliestqueue = e;
		}
	}
	earliesttime = min;
}




int main(void)
{
	//Initialize the run time
	int qa;
	int s, d, k;
	struct timeval time_start,  time_stop ;
    double time_pass;
 
	for(qa = 1; qa*100 < MAXQ; qa ++)
	{
		runtime[qa*100] = 0;
		gettimeofday(&time_start, NULL);
		//Initialization
		for(s = 0; s < qa*100; s++)
		{
			i[s] = 0;
			j[s] = 0;
			servstatus[s] = IDLE;
			nextarr[s] = INFINITE;
			nextdep[s] = INFINITE;
			qlength[s] = 0;
		}
		finish = 0;
		init();
		nextarr[0] = arr[0];
		while(finish < TOTALJOBS*2*qa*100)
		{
			earliest(qa);
			if(isarr)
			{
				arr_handler(earliestqueue, earliesttime);
			}
			else
			{
				dep_handler(earliestqueue, earliesttime, qa*100);
			}
			finish ++;
		}
		gettimeofday(&time_stop, NULL);
		runtime[qa*100] = time_pass = ((double)time_stop.tv_sec -(double)time_start.tv_sec)*1000000 + (double)time_stop.tv_usec-(double)time_start.tv_usec;
	}
	printf("N vs. run time (one '*' = 100usec)");
	for (d=1; d*100<MAXQ; d++){
		printf("\nN=%5d |",d*100);
		for (k=0; k*100 < runtime[d*100]; k++)
			printf("*");
	}

	getchar();
}
