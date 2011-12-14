
#include "TickTimer.h"

#include <stdlib.h>

TickTimer::TickTimer(int ticksPerSecond)
	: ticksPerSecond(ticksPerSecond)
{ }

TickTimer::~TickTimer()
{ }
	
void
TickTimer::start()
{
	gettimeofday(&last, NULL);
}

int
TickTimer::getTicks()
{
	struct timeval current;
	struct timeval delta;

	gettimeofday(&current, NULL);
	timersub(&current, &last, &delta);

	int secondTicks = delta.tv_sec * ticksPerSecond;
	int microsecondTicks = delta.tv_usec * ticksPerSecond / 1000000;

	delta.tv_usec = microsecondTicks * 1000000 / ticksPerSecond;
	timeradd(&last, &delta, &last);

	return secondTicks + microsecondTicks;
}

