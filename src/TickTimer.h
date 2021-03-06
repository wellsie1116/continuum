
#include <sys/time.h>

#ifndef TICKTIMER_H_36EU2152
#define TICKTIMER_H_36EU2152

class TickTimer {
public:
	TickTimer(int ticksPerSecond);
	~TickTimer();

	void start();
	int getTicks();

	float getTickRate();
	void setTickRate(float rate);

private:
	float mTickRate;
	int mTicksPerSecond;
	struct timeval mLast;
};

#endif

