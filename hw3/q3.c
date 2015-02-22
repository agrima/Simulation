#include "csim.h"
#include <stdio.h>
#define GREENAB 80 
#define GREENBA 60
#define NARS 3600
#define NARS1 3600
FACILITY greenab;             /*pointer for facility light green a to b */
FACILITY greenba;             /*pointer for facility light green b to a */
EVENT done1;             /*pointer for counter */
EVENT GA;             /*pointer for counter green a to b */
EVENT GB;           /*pointer for counter green b to a*/

EVENT done;             /*pointer for counter */
TABLE tbl;              /*pointer for table */
QTABLE qtbl;            /*pointer for qhistogram */
int cnt,cnt1;                /*number of active tasks*/
void gab();
void gba();
void generateA();
void generateB();
void event_handler();
void sim()                              /*1st process - named sim */
{

	set_model_name("TRAFFIC SIGNAL");
	create("sim");                          /*required create statement*/

	greenab = facility("greenab");               /*initialize facility*/
	greenba = facility("greenba");               /*initialize facility*/
	done = event("done");                   /*initialize event*/
	done1 = event("done1");                   /*initialize event*/
	GA = event("GA");                   /*initialize event*/
	GB = event("GB");                   /*initialize event*/
tbl = table("resp tms");                /*initialize table */
	qtbl = qhistogram("num in sys", 10l);   /*initialize qhistogram*/

	cnt = NARS/2;cnt1= NARS/2 ;
	hold(0);
	event_handler();
	generateA();
	generateB();
	wait(done);
//	event_handler();
	wait(done1);
//	event_handler();
 
//	printf("holding");
	// 		hold(GREENAB);
	 report();                               /*print report*/
	 mdlstat();
}
void event_handler(){
	create("event_handler");
	int j;
	for(j=0;j<NARS;j++ ){

	set(GA); /*green a to b*/
	hold(GREENAB);
	clear(GA);
	hold(55);/* both red*/
	set(GB);
	hold(GREENBA);/*green b to a*/
	clear(GB);

}	

}
void generateA(){	//generating cars from a to b
	create("generateA");
		int i,j;
      for(i = 1; i <= (NARS/2); i++) {
		hold(exponential(12));           
	//		for(j=0; j<GREENAB; j++){
//	printf("going to GAB");
//		use(greenab,2);
//	 event_handler();
	gab();                         
	//		}
//	printf("came out of GAB");
//	}
//		cnt--;
   

}
//	if(cnt==0)
//		set(done);    

}
void generateB(){	//generating cars from b to a
		create("generateB");
		int i,j;
        for(i = 1; i <=( NARS/2); i++) {
//		 printf("starting with another thread");
		 hold(exponential(9));            
//		use(greenba,2);
//		event_handler(); 
	 gba();                        

//}

    
}
	   
}
void gab()                   //cars crossing green light
{
	TIME t1;
	create("gab");                  
		wait(GA);
	t1 = clock;                 
	note_entry(qtbl);         
//	printf("inside GAB");
	use(greenab,2);                  
	record(clock-t1, tbl);
	note_exit(qtbl);                 
	cnt--;
	if(cnt==0)
		set(done);              
}

void gba()                           
{
	TIME t1;	//cars from b to a crossing green light
	int j;
	create("gba");                        
		wait(GB);
	t1 = clock;                            
	note_entry(qtbl);                       
	use(greenba,2);                         
	record(clock-t1, tbl);                
	note_exit(qtbl);                      
	cnt1--;
	if(cnt1==0)
		set(done1);                 
}

