#include <stdio.h>
#include <math.h>

#define NCUSTOMER 2000 //The number of customer per run

#define RUNPERVALUE 10 //The run times for each value of p

#define NVALUE 10 //The number of distinct value of p in the interval (0,1)

#define BUSY 1 //server status is busy

#define IDLE 0 //server status is idle

#define EXPSERVICE 1 //The expected value or mean of service time

#define INFINITE 1000000 //a very large value

double expon(double *, float);


int main() {

  int r,k,i,NW,j,w,status;
  double a,s,A,D,C,lastclock,clock,sumD,avgD,sumNW,avgNW,sumU,avgU,T,Q[NCUSTOMER],seed;
  double SUMD[NVALUE],AVGD[NVALUE],SUMNW[NVALUE],AVGNW[NVALUE],SUMU[NVALUE],AVGU[NVALUE];
  float expservice,nvalue=NVALUE;
  double quelen[RUNPERVALUE];
  double avequelen, svarquelen, sumquelen;

  /* 
  Variable declarations:
  i: The next customer be predicted. We predict next cutomer when all the customers before he are in the queue , serving, or served.
  NW: The total number of the customers in queue.
  j: The customer being served.
  status: The status of the server.
  a: A new interarrival time.
  s: A new service time.
  A: The next arrival event happen time.
  D: The deley time of the customer to be served just now.
  C: The next departure event happen time.
  lastclock: last event happen time.
  clock: this event happen time.
  sumD: the total deley time until now.
  avgD: the average delay time per customer in one run.
  sumNW: to compute the average number in system.
  avgNW: the average number in system in one rum.
  sumU: total busy time of server to compute the system utilization.
  avgU: the system utilization.
  T: interval between adjacent clock.
  Q[]: restore the arrival time of every customer.
  seed: the seed of random variant generator.
  */
  float exp_arrival[10] = {10.0, 5.0, 3.33, 2.5, 2, 1.6, 1.4, 1.2, 1.1,7.14 };

  float EXPARRIVAL;	


  for(w=0;w<10;w++){
  		
  	EXPARRIVAL = exp_arrival[w];


  for (k=0;k<NVALUE;k++){//each value of p
	  expservice=EXPSERVICE;
	  SUMD[k]=0;
	  SUMNW[k]=0;
	  SUMU[k]=0;
	  for (r=0;r<RUNPERVALUE;r++){//each run
		    //Initialization
		
		  seed = 1234567+r*100000+k*1000000; //set the seed independently for each run.
			i=0;
			clock=0;
			status=IDLE;
			NW=0;
			j=0;
			sumD=0;
			sumNW=0;
			sumU=0;
			a=expon(&seed,EXPARRIVAL); //generate a new interarrival time
			Q[0]=a;
			A=a;
			C=INFINITE;
			//End of initialization
			while (j<NCUSTOMER){	//there is any customer not be served yet	 
			lastclock=clock; //restore the last clock time when current clock time is going to jump to a new event happen time
			if (C<A){//A departure event happens
				j++; //point to the next customer to be served
				
				clock=C; //current clock time is going to jump to a new departure event happen time
				
				T=clock-lastclock; //compute the time between last clock and current clock
				sumU=sumU+T; //accumulate the total busy time of server
				if (NW!=0){//Queue is not empty
					sumNW=sumNW+T*NW; //accumulate sumNW with queue length multiplied by the time the length holds
					NW--; //queue length decrease 1
					
					D=clock-Q[j]; //Compute the delay of current customer
					
					sumD=sumD+D;	//accumulate the total delay
					s=expon(&seed,expservice); //generate the service time of current customer
					C=clock+s; //generate the departure time of current customer

				}
				else{
					//Queue is empty
					status=IDLE; //Change the server status into idle
					C=INFINITE; //all customers have been departed
				}
			}
			else{
				//An arrival event happens
						  
				clock=A; //current clock time is going to jump to a new arrival event happen time
					  
				if(status!=0){//Server status is busy
					T=clock-lastclock; //compute the time between last clock and current clock
					sumNW=sumNW+T*NW; //accumulate sumNW with queue length multiplied by the time the length holds
					NW++;	//queue length increase 1
				
					sumU=sumU+T; //accumulate the total busy time of server
					
					}
				else{
					//Server status is idle
					status=BUSY; //Change the server status into busy
					
					D=clock-Q[j]; //Compute the delay of current customer
					
					sumD=sumD+D; //accumulate the total delay
					s=expon(&seed,expservice); //generate the service time of current customer
					C=clock+s; //generate the departure time of current customer
				}
				i++; //point to the next arrive customer
				if (i<NCUSTOMER){	//there is any customer not arrived yet	  
					a=expon(&seed,EXPARRIVAL); //generate the interarrival time of next arrive customer
					Q[i]=Q[i-1]+a; //generate the arrival time of next arrive customer
					A=Q[i]; //the next arrival event happen time
					
				}
				else A=INFINITE; //all customers arrived
				}
			}
		}
	  avgNW=sumNW/clock; //the average number in system in one run
	  SUMNW[k]=SUMNW[k]+avgNW;
	  avgD=sumD/NCUSTOMER; //the average delay time per customer in one run
	  SUMD[k]=SUMD[k]+avgD;
	
	}
	}

printf("\n REPORT:");
      avgNW=sumNW/clock; //compute the average number in system in one rum
      printf("\n The average number in queue system is %f",avgNW);
      avgD=sumD/NCUSTOMER; //compute the average delay time per customer in one run
      printf("\n The average delay in system is %f",avgD);	
}

