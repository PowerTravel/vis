
#include "MainWindow.h"
#include <sstream>


bool MainWindow::INITIALIZED = false;

MainWindow::MainWindow()
{
	window = NULL;
	zombie = true;
	running = false;
	width = 640;
	height = 480;
	bpp = 32;
	header = "Main Window";
	countFPS = true;

	fpsLock = Timer();
	fpsLock.start();
	fpsCount = Timer();
	fpsCount.start();

	if( initGLFW() )
	{
		zombie = false;
		running = true;
	}
}

MainWindow::~MainWindow()
{
	glfwDestroyWindow(window);
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

		//glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Needed for osx
		
		INITIALIZED = true;
	}

	window = glfwCreateWindow(width, height, header.c_str(), NULL, NULL);

	if(window == NULL){
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	return true;
}

bool MainWindow::isZombie()
{
	return zombie;
}

bool MainWindow::isRunning()
{
	return running;
}

void MainWindow::getInput()
{
	glfwPollEvents();
}

void MainWindow::update()
{
	running = !glfwWindowShouldClose(window);
}

void MainWindow::draw()
{
	// Lock the program to FRAMES_PER_SECOND
	if( fpsLock.isStarted() && !fpsLock.isPaused() )
	{
		while(fpsLock.getTime()*FRAMES_PER_SECOND < 1.0 )
		{
			// Do nothing. Just wait.
		}
		fpsLock.start();
	}
	
	// Show fps in the title
	if( countFPS )
	{
		if( fpsCount.getTime() >= 1.0 )
		{
			std::stringstream title;
			title<< header << " FPS: " << frameCounter;
			std::string str = title.str();
			glfwSetWindowTitle( window, str.c_str() );
			frameCounter = 0;	
			fpsCount.start();
		}
	}

	frameCounter ++;
	glfwSwapBuffers(window);
}

Rect MainWindow::getWindowDim()
{
	Rect r;
	glfwGetWindowPos( window, &r.x, &r.y );
	glfwGetWindowSize( window, &r.w, &r.h );
	return r;
}

void MainWindow::setWindowDim(Rect nr)
{
	// If any of the parameters are invalid we set them to the current value
	Rect r = getWindowDim();
	if( nr.x<0 ){ nr.x = r.x; }
	if( nr.y<0 ){ nr.y = r.y; }
	if( nr.w<=0 ){ nr.w = r.w; }
	if( nr.h<=0){ nr.h = r.h; }

	glfwSetWindowSize(window, nr.w, nr.h);
	glfwSetWindowPos(window, nr.x, nr.y);
}
