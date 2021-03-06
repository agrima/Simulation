#include "sim.h"

facility::facility() {
	int i;
	status = FREE;
	waiting_tid = SNULL;
	cusnum = 0;
	for (i = 0; i < NARS_MAX; i ++) {
		arrtime[i] = INF;
		deptime[i] = INF;
	}
	delay = 0;
	queuelen = 0;
	servtime = 0;
}

void facility::reserve() {
	if (status == FREE) {
		status = OCCUPIED;
	}
	else {
		waiting_tid = (int)pthread_self();
		sched_yield();
	}
	cout << "thread "<<pthread_self()<<" gets facility at time "<<simclock<<endl;
}

void facility::release() {
	int i,j;
	if (waiting_tid != SNULL) {
		//insert the thread into calendar
		for (i = 0; i < THREAD_MAX; i ++) {
			if(simclock > calentry[i].occtime) {
				continue;
			}
			else {
				for (j = THREAD_MAX - 1; j > i; j --) {
					calentry[j].occtime = calentry[j-1].occtime;
					calentry[j].pid = calentry[j-1].pid;
				}
				calentry[j].occtime = simclock;
				calentry[j].pid = waiting_tid;
				break;
			}
		}
		//remove the waiting thread from the queue
		waiting_tid = SNULL;
	}
	else {
		status = FREE;
	}
	cout << "thread "<<pthread_self()<<" releases facility at time "<<simclock<<endl;
}

void facility::notice_arrive() {
	int i;
	for (i = 0; i < NARS_MAX; i ++) {
		if (arrtime[i] == INF) {
			break;
		}
	}
	arrtime[i] = simclock;
	cout<<"Thread "<<pthread_self()<<" arrives at arrtme["<<i<<"] = "<<arrtime[i]<<endl;
}

void facility::notice_leave() {
	int i;
	for (i = 0; i < NARS_MAX; i ++) {
		if (deptime[i] == INF) {
			break;
		}
	}
	deptime[i] = simclock;
	cout<<"Thread "<<pthread_self()<<" departs at deptime["<<i<<"] = "<<deptime[i]<<endl;
}

void facility::report() {
	int i;
	float sumdelay = 0;
	for (i = 0; i < NARS_MAX; i ++) {
		if (deptime[i] == INF) {
			break;
		}
		//cout << "arrtime["<<i<<"] = "<<arrtime[i]<<endl;
		//cout << "deptime["<<i<<"] = "<<deptime[i]<<endl;
		sumdelay += deptime[i] - arrtime[i];
		//cout << "sumdelay = "<<sumdelay<<endl;
		cusnum ++;
	}
	cout<<"\n=============== Report ==============="<<endl;
	cout<<"Total number of customers served is "<<cusnum<<endl;
	cout<<"Mean delay in system is "<<sumdelay/cusnum<<endl;
}
