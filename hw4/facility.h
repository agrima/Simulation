#define NARS_MAX 10

class facility {
public:
	facility();
	void reserve();
	void release();
	void report();
	void notice_arrive();
	void notice_leave();
private:
	int status;
	int cusnum;
	int waiting_tid;
	float arrtime[NARS_MAX];
	float deptime[NARS_MAX];
	float delay;
	float queuelen;
	float servtime;
};
