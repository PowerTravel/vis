#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Timer.hpp"

#ifndef _KEY__
#define _KEY__
typedef unsigned long keybit;
namespace KEY{
	const keybit nokey	= 0;
	const keybit q 		= 1;
};
#endif // _KEY__

class MainWindow
{	
	public:
		static MainWindow& getInstance();

		static void init(int w = 640, int h= 480, int fps = 60, std::string header = "Main Window");
		static void destroy();

		// Constructor and destructor
//		MainWindow();
//		virtual ~MainWindow();

		// Public members
		bool isRunning();

		void getInput();
		void update();
		void clear();
		void swap();



		GLFWwindow* getWindow();
		void getWindowSize(int* x, int* y, int* w, int* h);
		void setWindowSize(int x, int y, int w, int h);
		static void setClearColor(float r, float g, float b, float w);
		void setKeyCallback(void (*));
	private:
		MainWindow();
		virtual ~MainWindow();

		MainWindow(MainWindow const&) = delete;
		void operator=(MainWindow const&) = delete;

		// Fields
		static GLFWwindow* _window;
		
		static bool INITIALIZED;
		static keybit KEYBIT;

		static std::string _header;
		static bool _running;

		static Timer _fpsLock;
		static Timer _fpsCount;
		static int _FPS;
		static int _frameCounter;

		// Initiates glfw and creates a window.
		static bool initGLFW();
		static bool initGlew();

		static void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods );
		static void error_callback(int error, const char* error_message);
};



#endif // MAINWINDOW_HPP
