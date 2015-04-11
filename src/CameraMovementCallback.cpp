#include "CameraMovementCallback.hpp"
#include "MainWindow.hpp"
#include "MatLib.hpp"
#include <iostream>
CameraMovementCallback::CameraMovementCallback(camera_ptr cam)
{	
	_cam = cam;
	_angularVelocity = 0.02;
	_speed = 0.4;
}

CameraMovementCallback::~CameraMovementCallback()
{
}

void CameraMovementCallback::execute()
{
	GLFWwindow* window = MainWindow::getInstance().getWindow();

	if( glfwGetKey(window, GLFW_KEY_W)  == GLFW_PRESS) 
	{
		_cam->translate(vec3(0,0,-_speed));
	}
	if( glfwGetKey(window, GLFW_KEY_S)  == GLFW_PRESS) 
	{
		_cam->translate(vec3(0,0,_speed));
	}
	if( glfwGetKey(window, GLFW_KEY_A)  == GLFW_PRESS) 
	{
		_cam->translate(vec3(-_speed,0,0));
	}
	if( glfwGetKey(window, GLFW_KEY_D)  == GLFW_PRESS) 
	{
		_cam->translate(vec3(_speed,0,0));
	}
	if( glfwGetKey(window, GLFW_KEY_Z)  == GLFW_PRESS) 
	{
		_cam->translate(vec3(0,-_speed,0));
	}
	if( glfwGetKey(window, GLFW_KEY_X)  == GLFW_PRESS) 
	{
		_cam->translate(vec3(0,_speed,0));
	}
	if( glfwGetKey(window, GLFW_KEY_UP)  == GLFW_PRESS) 
	{
		_cam->rotate(_angularVelocity,vec3(1,0,0));
	}
	if( glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS) 
	{
		_cam->rotate(-_angularVelocity,vec3(1,0,0));
	}
	if( glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS) 
	{
		_cam->rotate(_angularVelocity,vec3(0,1,0));
	}
	if( glfwGetKey(window, GLFW_KEY_RIGHT)  == GLFW_PRESS) 
	{
		_cam->rotate(-_angularVelocity,vec3(0,1,0));
	}
	
	if( glfwGetKey(window, GLFW_KEY_R)  == GLFW_PRESS) 
	{
		_cam->lookAt(vec3(0.f,0.f,3.f),vec3(0.f,0.f,0.f),vec3(0.f, 1.f, 0.f) );
	}
}

