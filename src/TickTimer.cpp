
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

	int ticksPerSecond = mTicksPerSecond * mTickRate;

	if (!ticksPerSecond)
	{
		gettimeofday(&mLast, NULL);
		return 0;
	}

	gettimeofday(&current, NULL);
	timersub(&current, &mLast, &delta);

	long int secondTicks = delta.tv_sec * ticksPerSecond;
	long int microsecondTicks = delta.tv_usec * ticksPerSecond / 1000000;

	delta.tv_usec = microsecondTicks * 1000000 / ticksPerSecond;
	timeradd(&mLast, &delta, &mLast);

	return secondTicks + microsecondTicks;
}

float
TickTimer::getTickRate()
{
	return mTickRate;
}

void
TickTimer::setTickRate(float rate)
{
	mTickRate = rate;
}

