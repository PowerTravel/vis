#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Timer.hpp"

#ifndef KEY
#define KEY
typedef unsigned long keybit;
namespace KEY{
	const keybit nokey	= 0;
	const keybit q 		= 1;
};
#endif // KEY

class MainWindow
{	
	public:
		// Constructor and destructor
		MainWindow(int w = 640, int h= 480, int fps = 60, std::string header = "Main Window" );
		virtual ~MainWindow();

		// Public members
		bool isRunning();

		void getInput();
		void update();
		void clear();
		void swap();

		void get(GLFWwindow* window);

		void getWindowSize(int* x, int* y, int* w, int* h);
		void setWindowSize(int x, int y, int w, int h);

		void setClearColor(float r, float g, float b, float w);

		// Initiates glfw and creates a window.
		static bool initGLFW();

		void setKeyCallback(void (*));
	private:

		// Fields
		GLFWwindow* _window;
		
		static bool INITIALIZED;
		static KEY::keybit KEYBIT;

		std::string _header;
		bool _running;

		Timer _fpsLock;
		Timer _fpsCount;
		int _FPS;
		int _frameCounter;
		bool _countFPS;
		float _clearColor[4];


		static void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods );
		static void error_callback(int error, const char* error_message);
};



#endif // MAINWINDOW_HPP
