#include <iostream>
#include "MainWindow.hpp"
#include <sstream>


bool MainWindow::INITIALIZED = false;
KEY::keybit MainWindow::KEYBIT = KEY::nokey;

MainWindow::MainWindow(int w, int h, int fps, std::string header)
{
	_window = NULL;
	_running = false;
	_FPS = fps;
	_header = header;
	_countFPS = true;

	setClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	_fpsLock = Timer();
	_fpsLock.start();
	_fpsCount = Timer();
	_fpsCount.start();

	glfwSetErrorCallback(error_callback);
	if(initGLFW())
	{
		std::stringstream title;
		title<< _header << " FPS: " << _FPS;
		_window = glfwCreateWindow(w, h, title.str().c_str(), NULL, NULL);

		if(_window == NULL){
			glfwTerminate();
		}else{
			_running = true;
		}
	}

	glfwMakeContextCurrent(_window);

	glfwSetKeyCallback(_window, &MainWindow::key_callback);
}

MainWindow::~MainWindow()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}


bool MainWindow::initGLFW()
{
	// If this is the first window, we initialiez glfw with it.
	if( !INITIALIZED )
	{
		if( !glfwInit() ){
			return false;
		}

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Needed for osx
		
		INITIALIZED = true;
	}

	return true;
}

bool MainWindow::isRunning()
{
	return _running;
}

void MainWindow::getInput()
{
	glfwPollEvents();

	if((KEYBIT & KEY::q )== KEY::q)
	{
		glfwSetWindowShouldClose(_window, true);
	}
}

void MainWindow::update()
{
	_running = !glfwWindowShouldClose(_window);
}

void MainWindow::clear()
{
	glClearColor(_clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void MainWindow::swap()
{
	// Lock the program to FRAMES_PER_SECOND
	if( _fpsLock.isStarted() && !_fpsLock.isPaused() )
	{
		while(_fpsLock.getTime()*_FPS < 1.0 )
		{
			// Do nothing. Just wait.
		}
		_fpsLock.start();
	}
	
	// Show fps in the title
	if( _countFPS )
	{
		if( _fpsCount.getTime() >= 1.0 )
		{
			std::stringstream title;
			title<< _header << " FPS: " << _frameCounter;
			std::string str = title.str();
			glfwSetWindowTitle( _window, str.c_str() );
			_frameCounter = 0;	
			_fpsCount.start();
		}
	}

	_frameCounter ++;
	glfwSwapBuffers(_window);
}

void MainWindow::get(GLFWwindow* window)
{
	window = _window;
}

void MainWindow::getWindowSize(int* x, int* y, int* w, int* h)
{
	glfwGetWindowPos( _window, x, y );
	glfwGetWindowSize( _window, w, h );
}

void MainWindow::setWindowSize(int x, int y, int w, int h)
{
	glfwSetWindowSize( _window, w, h);
	glfwSetWindowPos( _window, x, y);
}

void MainWindow::setClearColor(float r, float g, float b, float w)
{
	_clearColor[0] = r;	
	_clearColor[1] = g;
	_clearColor[2] = b;
	_clearColor[3] = w;
}

void MainWindow::key_callback(GLFWwindow* w, int key, int scancode, int action, int mods )
{	
	if(key == GLFW_KEY_Q){
		if(action == GLFW_PRESS ){
			KEYBIT += KEY::q;
		}else if(action == GLFW_RELEASE){
			KEYBIT -= KEY::q;
		}
	}
}
void MainWindow::error_callback(int error, const char* error_message)
{
	std::cerr << error_message << std::endl;
}
