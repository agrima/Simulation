#include<csim.h>
#include<stdio.h>
#define ORDERS 91 //number of orders
#define RTIME 14400 /* 4 hours */
BUFFER prod;//production department acting like a buffer
FACILITY pck;//packing facility
EVENT done;
TABLE tbl;              /*pointer for table */
QTABLE qtbl;            /*pointer for qhistogram */
int p;

int c=0;

int lost;
void produce();
void pack();
//int widgets();
void generateOrders();//generating orders to be processed by packaging 

void sim(){
	set_model_name("Widget Production");
	create("sim");
	done=event("done");
	prod=buffer("prod",0);
	pck=facility("pck");
	tbl = table("resp tms");                /*initialize table */
        qtbl = qhistogram("num in sys", 10l);   /*initialize qhistogram*/
        p=ORDERS;
	lost = 0;//number of orders lost to WANT MORE.INC
        produce();
        generateOrders();
        wait(done);
        report();
        mdlstat();
	printf("total number of orders=%d\n",ORDERS);
        printf("total number of orders lost=%d\n",lost);
}

void generateOrders(){
        create("generateOrders");
        int i,cnt;
        for(i=0;i<ORDERS;i++){
                cnt=buffer_get_current_count(prod);//number of orders in buffer queue
                if(cnt<10){	//if buffer queue is more than 10 then adds up to lost
                	p--;
			pack();
			hold(expntl(300));
                }
		else
		{
			p--;
			lost++;
		}
		if(p==0)
			set(done);
	}
}


void produce() {
        create("produce");
	TIME t1;
	long int c, cnt1;
	int j;
	int max = ORDERS*7;
	int size;
        for(j=0;j<max;j++){
        	buffer_add_space(prod, 1);//adding space to buffer
		buffer_put(prod, 1)  ;//put widget in buffer
		hold(uniform(60,100));
	}
        csim_terminate();
}


void pack(){
        create("pack");
	TIME t1;
        double prob;int n;
        prob=uniform(0,1); 	/*to find the number of widgets*/
        if((prob>0) && (prob<=0.1))
                n=4;
        else if((prob>0.1) && (prob<=0.5))
                n=5;
        else if((prob>0.5) && (prob<=0.8))
                n=6;
        else
                n=7;
        t1 = clock;                             /*time of request */
        note_entry(qtbl);                       /*note arrival */
        buffer_get(prod, n);// to take out widget from bffer
        
        use(pck,(90+50*n));
	buffer_remove_space(prod,n);// removing packed widgets space from buffer

        record(clock-t1, tbl);                  /*record response time*/
        note_exit(qtbl);                        /*note departure */
}


