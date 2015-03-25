#include "Timer.h"

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

	startTime = glfwGetTime();
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
			return pausedTime;
		}else{
			return glfwGetTime() - startTime;
		}
	}

	return 0.0;
}

void Timer::pause()
{
	if( (started) && (!paused)  ){
		
		paused = true;
		pausedTime = glfwGetTime() - startTime;
	}
}

void Timer::unpause()
{
	if(paused){
		paused = false;

		startTime = glfwGetTime() - pausedTime;

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
