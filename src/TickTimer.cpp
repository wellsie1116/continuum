
#include "TickTimer.h"

#include <stdlib.h>

TickTimer::TickTimer(int ticksPerSecond)
	: mTicksPerSecond(ticksPerSecond)
	, mTickRate(1)
{ }

TickTimer::~TickTimer()
{ }
	
void
TickTimer::start()
{
	gettimeofday(&mLast, NULL);
}

int
TickTimer::getTicks()
{
	struct timeval current;
	struct timeval delta;

	gettimeofday(&current, NULL);
	timersub(&current, &mLast, &delta);

	int secondTicks = delta.tv_sec * mTicksPerSecond;
	int microsecondTicks = delta.tv_usec * mTicksPerSecond / 1000000;

	delta.tv_usec = microsecondTicks * 1000000 / mTicksPerSecond;
	timeradd(&mLast, &delta, &mLast);

	//FIXME incorporate tick rate sooner so ticks aren't delayed unnecessarily
	return (secondTicks + microsecondTicks) * mTickRate;
}

int
TickTimer::getTickRate()
{
	return mTickRate;
}

void
TickTimer::setTickRate(int rate)
{
	mTickRate = rate;
}

