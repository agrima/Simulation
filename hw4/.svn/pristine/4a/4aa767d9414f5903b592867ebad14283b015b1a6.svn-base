#include "sim.h"

#define	SVTM	3.0	/*mean of service time distribution */
#define IATM	5.0	/*mean of inter-arrival time distribution */
#define NARS 2

asim *s;
facility *f;
simevent *done;
int cnt = NARS;

void *sim(void*);
void *cust(void*);

main()
{
	pthread_t	   thread1, thread2;
	int rc = 0;
  void *status;
  struct sched_param param;
  pthread_attr_t attrA, attrB;

  pid_t mypid = getpid();
  struct sched_param schedparam;
	pthread_setschedprio(pthread_self(),MAXPRIO);
  
	/* bind process to processor 0 */
	/* unsigned long mask = 1;
	cpu_set_t cpu_info;
	CPU_ZERO(&cpu_info);
	CPU_SET(0, &cpu_info);
	if (sched_setaffinity(mypid, sizeof(mask), &cpu_info) <0) {
	  cout<<"sched_setaffinity error"<<endl;
	} */
  int policy = sched_getscheduler(mypid);
	sched_getparam( mypid, &schedparam);
  int newpolicy = SCHED_FIFO;
  schedparam.sched_priority = MAXPRIO; 
  sched_setscheduler(mypid, newpolicy,&schedparam);

  pthread_attr_init(&attrA);
  pthread_attr_init(&attrB);
  pthread_attr_setdetachstate(&attrA, PTHREAD_CREATE_JOINABLE);
  pthread_attr_setdetachstate(&attrB, PTHREAD_CREATE_JOINABLE);

  int thread_policy;
	int inheritsched;
	pthread_attr_getinheritsched(&attrA, &inheritsched);
	pthread_attr_getinheritsched(&attrB, &inheritsched);

	rc = pthread_attr_setinheritsched(&attrA, PTHREAD_EXPLICIT_SCHED); 
	if (rc!=0) { 
  	//printf("pthread_attr_setinheritsched() on attrA failed\n");
  	cout<<"pthread_attr_setinheritsched() on attrA failed"<<endl;
	} 
	rc = pthread_attr_setinheritsched(&attrB, PTHREAD_EXPLICIT_SCHED); 
	if (rc!=0) { 
  	//printf("pthread_attr_setinheritsched() on attrB failed\n");
  	cout<<"pthread_attr_setinheritsched() on attrB failed"<<endl;
	} 

  pthread_attr_getschedpolicy(&attrA, &thread_policy);

  thread_policy = SCHED_FIFO;

  if(0 != pthread_attr_setschedpolicy(&attrA, thread_policy) )
	//printf("pthread_attr_setschedpolicy() failed\n");
	cout<<"pthread_attr_setschedpolicy() failed"<<endl;
	  rc = pthread_attr_getschedparam(&attrA, &param);
	if( rc != 0 )
  	//printf("pthread_getschedparam() failed\n");
  	cout<<"pthread_getschedparam() failed"<<endl;
  	memset(&param, 0, sizeof(param));
  	param.sched_priority = MAXPRIO;
  	//printf("param.sched_priority for A = %d\n", param.sched_priority);
  	//cout<<"param.sched_priority for A = "<<param.sched_priority<<endl;
  	rc = pthread_attr_setschedparam(&attrA, &param);
	if( rc != 0 )
  	//printf("pthread_attr_setschedparam() failed\n");
  	cout<<"pthread_attr_setschedparam() failed"<<endl;
  	rc = pthread_create(&thread1, &attrA, sim, (void *)NULL);
	if( rc != 0 )
  	//printf("pthread_create() threadA failed\n");
  	cout<<"pthread_create() threadA failed"<<endl;

  pthread_attr_setschedpolicy(&attrB, thread_policy);
  rc = pthread_attr_getschedparam(&attrB, &param);
  memset(&param, 0, sizeof(param));
  param.sched_priority = MAXPRIO;
  //printf("param.sched_priority for B = %d\n", param.sched_priority);
  //cout<<"param.sched_priority for B = "<<param.sched_priority<<endl;
  rc = pthread_attr_setschedparam(&attrB, &param);
  rc = pthread_create(&thread2, &attrB, scheduler, (void *)NULL);

  rc = pthread_join( thread1, &status );
  rc = pthread_join( thread2, &status );

  pthread_attr_destroy(&attrA);
  pthread_attr_destroy(&attrB);

  pthread_exit(NULL);
}

void *sim(void* v) {
	int i;
	//cout << "sim() is thread "<<pthread_self()<<endl;
	s = new asim;
	f = new facility;
	done = new simevent;
	
	done->clear();
	
	for (i = 0; i < NARS; i++) {
	s->create(cust);
	s->hold(IATM);
	}
	done->wait();
	f->report();
}

void *cust(void* v)
{
	//cout << "cust() is thread "<<pthread_self()<<endl;
	f->notice_arrive();
	f->reserve();
	s->hold(SVTM);
	f->release();
	cnt --;
	f->notice_leave();
	if (cnt == 0) {
		done->set();
	}
}
