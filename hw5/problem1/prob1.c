#include "csim.h"
#include <stdio.h>
#include "utils.h"

#define RHO			0.95
#define	SVTM		1.0		/*mean of service time distribution */
#define NCUSTOMERS	5000	/*number of arrivals to be simulated*/
#define NRUNS 		25		/*number of runs to average over*/
#define NSERVERS 	8		/*number of servers in tandem queue system*/
#define SEED1 		988012135
#define SEED2 		671912980
#define PRIME1 		987653201
#define PRIME2 		36101521
#define PRIME3 		492574361
#define PRIME4 		67717379

FACILITY f[NSERVERS];		/*pointer for facilities */
EVENT done;					/*pointer for counter */
TABLE tbl;					/*pointer for table */
int cnt;					/*number of active tasks*/
void cust();
double seed1 = SEED1, seed2 = SEED2, iatm = SVTM/RHO;

void sim()					/*1st process - named sim */
{
	int i, j;
	char fac_name[] = "facility 00";

	set_model_name("Tandem Queue");
	create("sim");
	printf("%lf ",RHO);
	for(j = 0; j < NRUNS; ++j) {
		for(i = 0; i < NSERVERS; ++i) {
			fac_name[10] = i%10 + '0';
			fac_name[9] = (i/10)%10 + '0';
			f[i] = facility(fac_name);		/*initialize facility*/
		}
		done = event("done");			/*initialize event*/
		tbl = table("wait times");		/*initialize table */

		cnt = NCUSTOMERS;				/*initialize cnt*/
		for(i = 1; i <= NCUSTOMERS; i++) {
			hold(exponx(&seed1, iatm));		/*hold interarrival*/
			cust(i);				/*initiate process cust*/
		}
		wait(done);				/*wait until all done*/

		printf("%lf ", table_mean(tbl));

		/* Seed Refresh Begin */
		seed1 = fmod(seed1 + PRIME1, PRIME2);
		seed2 = fmod(seed2 + PRIME3, PRIME4);
		/* Seed Refresh End */
	}
	printf("\n");
}

void cust(int id)					/*process customer*/
{
	double t1, sum;
	int i;
 
	create("cust");					/*required create statement*/

	sum = 0.0;
	for(i = 0; i < NSERVERS; ++i){
		t1 = clock;					/*time of request */
		reserve(f[i]);				/*reserve facility f*/
		sum += (clock-t1);
		hold(exponx(&seed2, SVTM));	/*hold service time*/
		release(f[i]);				/*release facility f*/
	}
	record(sum, tbl);

	cnt--;							/*decrement cnt*/
	if(cnt == 0)
		set(done);					/*if last arrival, signal*/
}
