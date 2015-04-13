#include <iostream>
#include "MainWindow.hpp"
#include <sstream>


bool MainWindow::INITIALIZED = false;
keybit MainWindow::KEYBIT = KEY::nokey;

GLFWwindow* MainWindow::_window = NULL;
bool MainWindow::_running = false;
int MainWindow::_FPS = 60;
std::string MainWindow::_header;

Timer MainWindow::_fpsLock = Timer();
Timer MainWindow::_fpsCount = Timer();
int MainWindow::_frameCounter= 0;

MainWindow& MainWindow::getInstance()
{
	static MainWindow instance;
	return instance;
}


MainWindow::MainWindow()
{
}


MainWindow::~MainWindow()
{
}


void MainWindow::init(int w, int h, int fps, std::string header)
{
	if(!INITIALIZED)
	{
		if(initGLFW())
		_fpsLock.start();
		_fpsCount.start();
		_FPS = fps;
		_header = header;
		{
			std::stringstream title;
			title<< _header << " FPS: " << _FPS;
			_window = glfwCreateWindow(w, h, title.str().c_str(), NULL, NULL);
	
			if(_window == NULL){
				glfwTerminate();
				exit(EXIT_FAILURE);	
			}	
		}
	
		setClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glfwMakeContextCurrent(_window);
	
		glfwSetKeyCallback(_window, &MainWindow::key_callback);
	
		if(!initGlew())
		{
			exit(EXIT_FAILURE);	
		}else{
			_running = true;
			INITIALIZED = true;
		}
	}else{
		std::cerr<<"MainWindow already created." << std::endl;
	}

}

bool MainWindow::initGLFW()
{
	glfwSetErrorCallback(error_callback);
	// If this is the first window, we initialiez glfw with it.
	if( !glfwInit() ){
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Needed for osx

	return true;
}

bool MainWindow::initGlew()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (glewInit() != GLEW_OK){
	    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return false;
	}
	
	glLineWidth(1.0);
	glPointSize(1.0);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	return true;
}

void MainWindow::destroy()
{
	if(INITIALIZED)
	{
		glfwDestroyWindow(_window);
		glfwTerminate();
	}
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void MainWindow::swap()
{
	// Lock the program to FRAMES_PER_SECOND
	if( _fpsLock.isStarted() && !_fpsLock.isPaused() )
	{
	/*
		if(_fpsLock.getTime()*_FPS >= 1.0)
		{
			std::cerr << "MainWindow::FPS_DROP!" << std::endl;
		}
	*/
		while(_fpsLock.getTime()*_FPS < 1.0 )
		{
			// Do nothing. Just wait.
		}
		_fpsLock.start();
	}
	
	// Show fps in the title
	if( _fpsCount.getTime() >= 1.0 )
	{
		std::stringstream title;
		title<< _header << " FPS: " << _frameCounter;
		std::string str = title.str();
		glfwSetWindowTitle( _window, str.c_str() );
		_frameCounter = 0;	
		_fpsCount.start();
	}

	_frameCounter ++;
	glfwSwapBuffers(_window);
}

GLFWwindow* MainWindow::getWindow()
{
	return _window;
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
	glClearColor(r,g,b,w);
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
