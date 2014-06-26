// ### HEADER ### //

#ifndef _TIMER_H_
#define _TIMER_H_

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

class Timer
{
private:
	double mtime;
	struct timeval started, ended;
public:
	Timer()
	{
		start();
	}
	void start()
	{
		mtime = 0;
		gettimeofday(&started, NULL);
	}
	void stop()
	{
		gettimeofday(&ended, NULL);
		mtime += ((ended.tv_sec  - started.tv_sec)
				* 1000 + (ended.tv_usec - started.tv_usec) / 1000.0) + 0.5;
	}
	double getUnitTime()
	{
		return mtime;
	}
	double getUnitsPerSecond()
	{
		return 1000.0 / mtime;
	}
};

#endif
