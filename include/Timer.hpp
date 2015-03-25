#ifndef TIMER_HPP
#define TIMER_HPP

#include <GLFW/glfw3.h>

class Timer
{
	public:
		Timer();
		virtual ~Timer();

		void start();
		void stop();
		void pause();
		void unpause();
		float getTime();

		bool isStarted();
		bool isPaused();

	private:
		double startTime;
		double pausedTime;
		bool started;
		bool paused;
};


#endif // TIMER_HPP
