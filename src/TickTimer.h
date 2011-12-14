
#include <sys/time.h>

#ifndef TICKTIMER_H_36EU2152
#define TICKTIMER_H_36EU2152

class TickTimer {
public:
	TickTimer(int ticksPerSecond);
	~TickTimer();

	void start();
	int getTicks();

private:
	int ticksPerSecond;
	struct timeval last;
};

#endif

