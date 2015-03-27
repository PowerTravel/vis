#include "Timer.hpp"
#include <ctime>

Timer::Timer()
{
	startTime = 0.0;
	pausedTime = 0.0;
	started = false;
	paused = false;
}

Timer::~Timer()
{

}

void Timer::start()
{
	started = true;
	paused = false;

	startTime = std::clock();
}

void Timer::stop()
{
	started = false;
	paused = false;
}

float Timer::getTime()
{
	if(started){
		if(paused){
			return pausedTime/1000000.f;
		}else{
			return (std::clock() - startTime)/1000000.f;
		}
	}

	return 0.0;
}

void Timer::pause()
{
	if( (started) && (!paused)  ){
		
		paused = true;
		pausedTime = std::clock() - startTime;
	}
}

void Timer::unpause()
{
	if(paused){
		paused = false;

		startTime = std::clock() - pausedTime;

		pausedTime = 0.0;
	}
}

bool Timer::isStarted()
{
	return started;
}

bool Timer::isPaused()
{
	return paused;
}
