#include <stdio.h>
#include <stdlib.h>

double expon(double *, float);

#define NTYPE1 1000			//The number of type1 customer per run
#define NTYPE2 1000			//The number of type2 customer per run
#define EXPARRIV1 10		//The expected interarrival time of type1
#define EXPSERVICE 5		//The expected service time
#define INFINITE 1000000	//Indicate a very large value
#define IDLE 0
#define BUSY1 1
#define BUSY2 2
#define RUN 20				//Run times for each value of lambda1 or lambda2
#define NVALUE 10

//define a structure for type1 customer
struct type1 {
	double arrtime;	//arrival time
};

//define a structure for type1 customer
struct type2 {
	double arrtime;	//arrival time
	double remain; //remaining service time
};

//Two type of customers
struct type1 high[NTYPE1];
struct type2 low[NTYPE2];


double earliest(double a, double b, double c, double d)
{
	double min;
	if (a<b) min = a;
	else min = b;
	if (min>c) min = c;
	if (min>d) min = d;
	return min;
}

void init(double s, double a1, double a2)
{
	//Initialize the arrival time for all the customers
	int i,j;
	high[0].arrtime = expon(&s,a1);
	//printf("No. %d type1 customer has arrived at %f\r\n", 0, high[0].arrtime);
	low[0].arrtime = expon(&s,a2);
	//printf("No. %d type2 customer has arrived at %f\r\n", 0, low[0].arrtime);
	for(i=1; i<NTYPE1; i++)
	{
		high[i].arrtime = high[i-1].arrtime + expon(&s,a1);
		//printf("No. %d type1 customer has arrived at %f\r\n", i, high[i].arrtime);
	}
	for(j=1; j<NTYPE2; j++)
	{
		low[j].arrtime = low[j-1].arrtime + expon(&s,a2);
		low[j].remain = 0;
		//printf("No. %d type2 customer has arrived at %f\r\n", j, low[j].arrtime);
	}
}
int main(void)
{
	int i;					//index of type1 customer being served
	int j;					//index of type2 customer being served or being preempted
	int ii;					//index of type1 customer to arrive
	int jj;					//index of type2 customer to arrive
	double sumserv1;		//sum of service time for type1
	double sumserv2;		//sum of service time for type2
	double interval;		//time past before now and last event
	double nextarrive1;			//next type1 arrival time
	double nextdepart1;			//next type1 departure time
	double nextarrive2;			//next type2 arrival time
	double nextdepart2;			//next type2 departure time
	double lambda1;				//arrival rate of type1
	double lambda2;				//arrival rate of type2
	double mu;					//service rate
	double rho1;				//rho1 = lambda1/mu
	double rho2;				//rho2 = lambda2/mu
	int run, dis;
	double NUM1[NVALUE];
	double WAIT1[NVALUE];
	double SERV1[NVALUE];
	double NUM2[NVALUE];
	double WAIT2[NVALUE];
	double SERV2[NVALUE];
	double EXPARRIV2[NVALUE];
	double seed;
	double clock;			//record the current time
	double lastclock;		//record the time of last event
	double qlen1;			//length of queue of type1
	double qlen2;			//length of queue of type2
	int servstatus;			//server status is IDLE to begin with
	double sumnum1;			//sum of type1 number
	double sumnum2;			//sum of type2 number
	double sumwait1;		//sum of type1 waiting time
	double sumwait2;		//sum of type2 waiting time
	
	int d, k;

	lambda1 = 1.0/EXPARRIV1;
	mu = 1.0/EXPSERVICE;
	rho1 = lambda1/mu;

	for(dis = 0; dis < NVALUE; dis ++)
	{
		NUM1[dis] = 0;
		WAIT1[dis] = 0;
		SERV1[dis] = 0;
		NUM2[dis] = 0;
		WAIT2[dis] = 0;
		SERV2[dis] = 0;
		EXPARRIV2[dis] = 11 + dis;
		lambda2 = 1.0/EXPARRIV2[dis];
		rho2 = lambda2/mu;

		for(run = 0; run < RUN; run ++)
		{
			seed = 1234567+run*100000+dis*10000;
			init(seed, EXPARRIV1, EXPARRIV2[dis]);
			nextarrive1 = high[0].arrtime;	//next type1 arrival time
			nextdepart1 = INFINITE;		//next type1 departure time
			nextarrive2 = low[0].arrtime;	//next type2 arrival time
			nextdepart2 = INFINITE;		//next type2 departure time

			i = 0;				//index of type1 customer being served
			j = 0;				//index of type2 customer being served or being preempted
			ii = 0;				//index of type1 customer to arrive
			jj = 0;				//index of type2 customer to arrive
			clock = 0;			//record the current time
			lastclock = 0;			//record the time of last event
			qlen1 = 0;		//length of queue of type1
			qlen2 = 0;		//length of queue of type2
			servstatus = IDLE;	//server status is IDLE to begin with
			sumnum1 = 0;		//sum of type1 number
			sumnum2 = 0;		//sum of type2 number
			sumwait1 = 0;		//sum of type1 waiting time
			sumwait2 = 0;		//sum of type2 waiting time
			sumserv1 = 0;		//sum of service time for type1
			sumserv2 = 0;		//sum of service time for type2
			interval = 0;		//time past before now and last event

			while((i < NTYPE1)&&(j < NTYPE2))
			{
				lastclock = clock;				//record the time of last event
				/***********************	Type 1 Arrival Event Handler	***********************/	
				if (earliest(nextarrive1, nextdepart1, nextarrive2, nextdepart2) == nextarrive1)
				{
					clock = nextarrive1;
					interval = clock-lastclock;	//compute the time between last clock and current clock
					ii ++;						//next type1 customer to arrive
					if(servstatus == IDLE)
					{
						servstatus = BUSY1;		//Change the server status into busy
						nextdepart1 = clock + expon(&seed,EXPSERVICE);
						sumnum1 += qlen1 * interval;
						sumnum2 += qlen2 * interval;
					}
					else if (servstatus == BUSY1)
					{
						sumnum1 += qlen1 * interval;	//accumulate sumnum1 with queue length multiplied by the time the length holds
						sumnum2 += qlen2 * interval;	//accumulate sumnum2 with queue length multiplied by the time the length holds
						qlen1 = qlen1 + 1.0;
						sumserv1 += interval;	//accumulate the total busy time of server serving type1
					}
					else
					{
						low[j].remain -= interval;			//remain service time for type2 customer
						servstatus = BUSY1; //Change the server status into busy
						sumnum1 += qlen1 * interval;
						sumnum2 += qlen2 * interval;
						qlen2 = qlen2 + 1.0;
						sumserv2 += interval;		//accumulate the total busy time of server serving type2
						nextdepart1 = clock + expon(&seed,EXPSERVICE);
						nextdepart2 = INFINITE;
					}
					if(ii<NTYPE1)	//more type1 customers to arrive
						nextarrive1 = high[ii].arrtime;
					else			//all type1 customers as arrived
						nextarrive1 = INFINITE;
				}

				/***********************	Type 1 Departure Event Handler	***********************/
				if (earliest(nextarrive1, nextdepart1, nextarrive2, nextdepart2) == nextdepart1)
				{
					clock = nextdepart1;
					sumwait1 += clock - high[i].arrtime;	//add the waiting time (depart time - arrive time) for this type1 customer
					i ++;	//move to next type1 customer
					interval = clock - lastclock;
					sumserv1 += interval;		//accumulate the total busy time of server serving type1
					if ((qlen1 == 0) && (qlen2 == 0))	//if no one is in queue
					{
						servstatus = IDLE;
						nextdepart1 = INFINITE;
						nextdepart2 = INFINITE;
						sumnum1 += qlen1 * interval;
						sumnum2 += qlen2 * interval;
					}
					else if (qlen1 != 0)				//if type1 queue is not empty
					{
						sumnum1 += qlen1 * interval;	//accumulate sumnum1 with queue length multiplied by the time the length holds
						sumnum2 += qlen2 * interval;	//accumulate sumnum2 with queue length multiplied by the time the length holds
						qlen1 = qlen1 - 1.0;
						servstatus = BUSY1;
						nextdepart1 = clock + expon(&seed,EXPSERVICE);
						nextdepart2 = INFINITE;
					}
					else									//if type1 queue is empty but type2 queue is not empty
					{
						sumnum1 += qlen1 * interval;
						sumnum2 += qlen2 * interval;	//accumulate sumnum2 with queue length multiplied by the time the length holds
						qlen2 = qlen2 - 1.0;
						servstatus = BUSY2;
						nextdepart1 = INFINITE;
						if(low[j].remain == 0)				//a type2 customer who has never be preempted
						{
							low[j].remain = expon(&seed,EXPSERVICE);	//when service starts, the remaining service time is the total service time
							nextdepart2 = clock + low[j].remain;		//predicts the departure time for type2 customer if not preempted again
						}
						else								//a type2 customer has been preempted and put back in the queue
						{
							nextdepart2 = clock + low[j].remain;		//predicts the departure time for type2 customer if not preempted again
						}
					}
				}

				/***********************	Type 2 Arrival Event Handler	***********************/
				if (earliest(nextarrive1, nextdepart1, nextarrive2, nextdepart2) == nextarrive2)
				{
					clock = nextarrive2;
					interval = clock-lastclock;		//compute the time between last clock and current clock
					jj ++;						//next type2 customer to arrive
					if(servstatus == IDLE)
					{
						servstatus = BUSY2; //Change the server status into busy
						nextdepart2 = clock + expon(&seed,EXPSERVICE);
						sumnum1 += qlen1 * interval;
						sumnum2 += qlen2 * interval;
					}
					else if(servstatus != BUSY1)
					{
						sumnum1 += qlen1 * interval;
						sumnum2 += qlen2 * interval;	//accumulate sumnum2 with queue length multiplied by the time the length holds
						qlen2 = qlen2 + 1.0;
						sumserv2 += interval;	//accumulate the total busy time of server serving type2
						
					}
					else
					{ 
						sumnum1 += qlen1 * interval;	//accumulate sumnum1 with queue length multiplied by the time the length holds
						sumnum2 += qlen2 * interval;	//accumulate sumnum2 with queue length multiplied by the time the length holds
						qlen2 = qlen2 + 1.0;
						sumserv1 += interval;	//accumulate the total busy time of server serving type1

					}
					if(jj<NTYPE2)	//more type2 customers to arrive
						nextarrive2 = low[jj].arrtime;
					else			//all type2 customers has arrived
						nextarrive2 = INFINITE;
				}

				/***********************	Type 2 Departure Event Handler	***********************/
				if (earliest(nextarrive1, nextdepart1, nextarrive2, nextdepart2) == nextdepart2)
				{
					clock = nextdepart2;
					sumwait2 += clock - low[j].arrtime;	//add the waiting time (depart time - arrive time) for this type2 customer
					j ++;	//move to next type2 customer
					interval = clock - lastclock;
					sumserv2 += interval;		//accumulate the total busy time of server serving type2
					nextdepart1 = INFINITE;
					if (qlen2 != 0)			//if no one is in queue2
					{
						sumnum1 += qlen1 * interval;
						sumnum2 += qlen2 * interval;	//accumulate sumnum2 with queue length multiplied by the time the length holds
						qlen2 = qlen2 - 1.0;
						servstatus = BUSY2;
						low[j].remain = expon(&seed,EXPSERVICE);	//when service starts, the remaining service time is the total service time
						nextdepart2 = clock + low[j].remain;		
					}
					else						//if type2 queue is not empty
					{

						servstatus = IDLE;
						nextdepart2 = INFINITE;
						sumnum1 += qlen1 * interval;
						sumnum2 += qlen2 * interval;
//predicts the departure time for type2 customer if not preempted again
					}
				}
			}
			NUM1[dis] += sumnum1/clock;
			NUM2[dis] += sumnum2/clock;
			WAIT1[dis] += sumwait1/NTYPE1;
			WAIT2[dis] += sumwait2/NTYPE2;
		}

		

		NUM1[dis] = NUM1[dis]/RUN;
		NUM2[dis] = NUM2[dis]/RUN;
		WAIT1[dis] = WAIT1[dis]/RUN;
		WAIT2[dis] = WAIT2[dis]/RUN;

	}
	

	getchar();
}
