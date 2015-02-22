/* fixing ITAM1=10 while vary ITAM0 from 11~20
*/

#include "csim.h"
#include <stdio.h>

#define	SVTM	5.0	/*mean of service time distribution */
//#define IATM	5.0	/*mean of inter-arrival time distribution */
#define NARS	400	/*number of arrivals to be simulated*/
#define RUN 30			//Run times for each value of lambda1 or lambda2
#define NVALUE 10

FACILITY f;		/*pointer for facility */
EVENT done;		/*pointer for counter */
TABLE tbl;		/*pointer for table */
QTABLE qtbl;	/*pointer for qhistogram */
CLASS cl[2];	/* pointers to classes */
FILE *fp;
void cust();
void make_facility_report();
double p;		/*Probability that customer is type0	*/
double iatm;	/*mean of inter-arrival time distribution */
double NUM0[NVALUE];
double NUM1[NVALUE];
double WAIT0[NVALUE];
double WAIT1[NVALUE];

void sim()				/*1st process - named sim */
{
	int dis, run, i, d, k;
	double sumnum0, sumnum1, sumwait0, sumwait1;

	fp = fopen("q12report.txt", "w");
	set_output_file(fp);
	set_model_name("M/M/1 Queue");
	create("sim");						/*required create statement	*/

	f = facility_ms("facility", 1l);	/*initialize facility		*/
	collect_class_facility(f);			/*collect class statistics	*/
	tbl = table("resp tms");				/*initialize table 		*/
	qtbl = qhistogram("num in sys", 10l);	/*initialize qhistogram	*/
	cl[0] = process_class("class 0");		/*initialize cust class */
	cl[1] = process_class("class 1");

	set_servicefunc(f, pre_res); //set facility function to preempt&resume
	
	/*************	10 distinct values of exparriv2	*************/
	for(dis = 0; dis < NVALUE; dis ++) {
		p = 10.0/(21.0+dis);
		//printf("p = %f\r\n", p);
		iatm = (1.0-p)*10.0;
		//printf("iatm = %f\r\n", iatm);
		/*************	30 Runs for each exparriv2	*************/
		sumnum0 = 0;		//sum of type0 number
		sumnum1 = 0;		//sum of type1 number
		sumwait0 = 0;		//sum of type0 waiting time
		sumwait1 = 0;		//sum of type1 waiting time
		for(run = 0; run < RUN; run ++)
		{
			for(i = 0; i < NARS; i ++) {
				job(p);
				//printf("iatm = %f\r\n", iatm);
				hold(expntl(iatm));
			}
			wait(event_list_empty);
			sumnum0 += class_qlen(f, cl[0]);
			sumnum1 += class_qlen(f, cl[1]);
			sumwait0 += class_resp(f, cl[0]);
			sumwait1 += class_resp(f, cl[1]);
		}
		NUM0[dis] = sumnum0/RUN;
		//printf("%f\r\n", NUM0[dis]);
		NUM1[dis] = sumnum1/RUN;
		//printf("%f\r\n", NUM1[dis]);
		WAIT0[dis] = sumwait0/RUN;
		//printf("%f\r\n", sumwait0[dis]);
		WAIT1[dis] = sumwait1/RUN;
		//printf("%f\r\n", sumwait1[dis]);
	}
	printf("\r\n ITAM0 vs. average # of type0 jobs");
	printf(" in the system (one '*' = 0.2)");
	for (d=0; d<NVALUE; d++){
		printf("\n  %d |",11+d);
		for (k=0; k*0.2 < NUM0[d]; k++)
			printf("*");
	}
	printf("\r\n\r\n ITAM0 vs. average # of type1 jobs");
	printf(" in the system (one '*' = 0.02)");
	for (d=0; d<NVALUE; d++){
		printf("\n  %d |",11+d);
		for (k=0; k*0.02 < NUM1[d]; k++)
			printf("*");
	}
	printf("\r\n\r\n ITAM0 vs. average time spent in system by");
	printf(" type0 jobs (one '*' = 2)");
	for (d=0; d<NVALUE; d++){
		printf("\n  %d |",11+d);
		for (k=0; k*2 < WAIT0[d]; k++)
			printf("*");
	}
	printf("\r\n\r\n ITAM0 vs. average time spent in system by");
	printf(" type1 jobs (one '*' = 0.2)");
	for (d=0; d<NVALUE; d++){
		printf("\n  %d |",11+d);
		for (k=0; k*0.2 < WAIT1[d]; k++)
			printf("*");
	}
	printf("\r\n\r\n");
	report();				/*print report*/
}

job(double prob) {
	create("job");				/*required create statement*/
	TIME t1;
	int type;
	int temp;
	temp = random(0l, 10000l);
	if (temp < 10000*prob) {
		type = 0;
	}
	else {
		type = 1;
	}
	set_process_class(cl[type]);
	set_priority(type+1);
	t1 = clock;				/*time of request */
	note_entry(qtbl);			/*note arrival */
	//printf("SVTM = %f\r\n", SVTM);
	use(f,expntl(SVTM));  	//pack all the preempt&resume work into use
	record(clock-t1, tbl);			/*record response time*/
	note_exit(qtbl);			/*note departure */
}
