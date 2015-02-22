
#include <stdio.h>

#define NCUSTOMER 2000 //The number of customer per run

#define RUNPERVALUE 10 //The rum times for each mean vacation time E[v]

#define NVALUE 10 //The number of mean vacation time E[v]

#define INTERVAL 1//The interval of mean vacation time E[v]

#define MINVALUE 1//The minimal mean vacation time E[v]

#define BUSY 1 //Indicate the server status is busy

#define VACATION 0 //Indicate the server status is vacation

#define EXPARRIVAL 1.1 //The expected value or mean of interarrival time

#define EXPSERVICE 1 //The expected value or mean of service time

#define INFINITE 1000000 //Indicate a very large value

double expon(double *, float);


int main() {

		int r,k,i,w,NW,j,status;
		double a,s,v,V,A,D,C,lastclock,clock,sumD,avgD,sumNW,avgNW,sumU,avgU,T,Q[NCUSTOMER],seed;
		double SUMD[NVALUE],AVGD[NVALUE],SUMNW[NVALUE],AVGNW[NVALUE],SUMU[NVALUE],AVGU[NVALUE];
		float expvacation;
		int exp_vacation[10] = {0,1,2,3,4,5,6,7,8,9 };

		int EXPVACATION;


		for(w=0;w<10;w++){

				EXPVACATION = exp_vacation[w];

				/* 
				   Variable declarations:
i: The next customer be predicted. We predict next cutomer when all the customers before he are in the queue , serving, or served.
NW: The total number of the customers in queue.
j: The customer being served.
status: The status of the server.
a: A new interarrival time.
s: A new service time.
v: A new server vacation time.
V: The next end-of-server-vacation event happen time
A: The next arrival event happen time.
D: The deley time of the customer to be served just now.
C: The next departure event happen time.
lastclock: last event happen time.
clock: this event happen time.
sumD: the total deley time until now.
avgD: the average delay time per customer in one run.
sumNW: to compute the average number in system.
avgNW: the average number in system in one rum.
T: interval between adjacent clock.
Q[]: restore the arrival time of every customer.
seed: the seed of random variant generator.
				 */

				for (k=0;k<NVALUE;k++){//each value of E[v]
						expvacation=k*INTERVAL+MINVALUE;
					
						SUMD[k]=0;
						SUMNW[k]=0;
						SUMU[k]=0;
						for (r=0;r<RUNPERVALUE;r++){//each run
								//Initialization
								seed = 1234567+r*100000+k*1000000; //set the seed independently for each run.
								i=0;
								clock=0;
								status=VACATION;
								NW=0;
								j=0;
								sumD=0;
								sumNW=0;
								sumU=0;
								a=expon(&seed,EXPARRIVAL); //generate a new interarrival time
								Q[0]=a;
								A=a;
								
								V=0;
								C=INFINITE;
								//End of initialization
								while (j<NCUSTOMER){	//there is any customer not be served yet	 
										lastclock=clock; //restore the last clock time when current clock time is going to jump to a new event happen time
										if (C>A&&V>A){//An arrival event happens
												
												clock=A; //current clock time is going to jump to a new arrival event happen time
												
												T=clock-lastclock; //compute the time between last clock and current clock
												sumNW=sumNW+T*NW; //accumulate sumNW with queue length multiplied by the time the length holds
												NW++;	//queue length increase 1
												
												if(status==1){//Server status is busy
														sumU=sumU+T; //accumulate the total busy time of server
												}
												i++; //point to the next arrive customer
												if (i<NCUSTOMER){	//there is any customer not arrived yet	  
														a=expon(&seed,EXPARRIVAL); //generate the interarrival time of next arrive customer
														Q[i]=Q[i-1]+a; //generate the arrival time of next arrive customer
														A=Q[i]; //the next arrival event happen time
														
												}
												else A=INFINITE; //all customers have been arrived
												continue;
										}

										if (A>C&&V>C){//A departure event happens
												j++; //point to the next customer to be served
												
												clock=C; //current clock time is going to jump to a new departure event happen time
												
												T=clock-lastclock; //compute the time between last clock and current clock
												sumU=sumU+T; //accumulate the total busy time of server
												if (NW==0){//Queue is empty
														status=VACATION; //Change the server status into vacation
														
														C=INFINITE; //all customers have been departed
														v=expon(&seed,expvacation); //generate the vacation time
														V=clock+v; //compute the end-of-server-vacation event happen time
														
												}
												else{//Queue is not empty
														sumNW=sumNW+T*NW; //accumulate sumNW with queue length multiplied by the time the length holds
														NW--; //queue length decrease 1
														
														D=clock-Q[j]; //the delay of current customer
														
														sumD=sumD+D;	//accumulate the total delay
														s=expon(&seed,EXPSERVICE); //generate the service time of current customer
														C=clock+s; //compute the departure time of current customer
														
												}
												continue;
										}

										if (A>V&&C>V){//An end-of-server-vacation event happens
												
												clock=V; //current clock time is going to jump to a new end-of-server-vacation event happen time
												
												if (NW==0){//Queue is empty
														
														v=expon(&seed,expvacation); //generate the vacation time
														V=clock+v; //compute the end-of-server-vacation event happen time
														
												}
												else{//Queue is not empty
														V=INFINITE; //Server is not on vacation
														status=BUSY; //Change the server status into busy
														
														T=clock-lastclock; //the time between last clock and current clock
														sumNW=sumNW+T*NW; //accumulate sumNW with queue length multiplied by the time the length holds
														NW--;	//queue length decrease 1
														
														D=clock-Q[j]; //the delay of current customer
														
														sumD=sumD+D; //accumulate the total delay
														s=expon(&seed,EXPSERVICE); //generate the service time of current customer
														C=clock+s; //generate the departure time of current customer
														
												}
												continue;
										}
								}
								
								avgNW=sumNW/clock; //the average number in system in one run
								SUMNW[k]=SUMNW[k]+avgNW;
								avgD=sumD/NCUSTOMER; //the average delay time per customer in one run
								SUMD[k]=SUMD[k]+avgD;
								
						}

				}
		}

}
