#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include "facility.h"
#include "simevent.h"
#include "asim.h"
#include "calendar.h"
#include <unistd.h>
#define THREAD_MAX 10
#define INF 100000000
#define FREE 0
#define OCCUPIED 1
#define SNULL -1

#define MAXPRIO sched_get_priority_max(SCHED_FIFO)
#define MINPRIO sched_get_priority_min(SCHED_FIFO)

using namespace std;

extern float simclock;
extern int last_thread;

extern void* scheduler(void *);
extern calendar calentry[];
