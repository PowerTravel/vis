#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include "MainWindow.hpp"

#include "Shader.hpp"

#include "Geometry.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "RenderVisitor.hpp"
#include "UpdateVisitor.hpp"
#include "State.hpp"

#include "CameraMovementCallback.hpp"
#include <memory>
#include <map>
#include <vector>

#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

void setUpGlew()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (glewInit() != GLEW_OK){
	    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	glLineWidth(1.0);
	glPointSize(1.0);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	//glFrontFace(GL_CW);
}

GLFWwindow* createWindow()
{
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return NULL;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow( 1024, 768, "Main Window", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	
	setUpGlew();
	
	return window;
}



group_ptr build_graph()
{

	shader_ptr s = shader_ptr(new Shader("../shaders/vshader.glsl", "../shaders/fshader.glsl"));
	State state = State();
	state.set(State::Attribute::SHADER,s);

	geometry_vec gvec = Geometry::loadFile("../models/box.obj");
	group_ptr grp = group_ptr(new Group());
	grp->setState(&state);

	camera_ptr cam = camera_ptr(new Camera());
	cam->connectCallback(callback_ptr(new CameraMovementCallback(cam)));
	grp->addChild(cam);

	transform_ptr trns1 = transform_ptr(new Transform());
	trns1->translate(vec3(0.5,0,0));
	trns1->scale(vec3(0.5,0.5,0.5));
	trns1->rotate(90,vec3(0,0,1));
	state = State();
	state.set(State::Attribute::RENDER_MODE, State::Value::LINE);
	state.set(State::Attribute::BACK_FACE_CULLING, State::Value::OFF);
	trns1->setState(&state);

	transform_ptr trns2 = transform_ptr(new Transform());
	trns2->translate(vec3(-0.5,0,0));
	trns2->scale(vec3(0.5,0.5,0.5));

	for(int i = 0; i<gvec.size(); i++){
		trns1->addChild(gvec[i]);
		trns2->addChild(gvec[i]);
	}

	cam->addChild(trns1);
	cam->addChild(trns2);

	return grp;

/*
	geometry_vec gvec = Geometry::loadFile("../models/box.obj");
	group_ptr grp = group_ptr(new Group());
	for(int i = 0; i<gvec.size(); i++){
		grp->addChild(gvec[i]);
	}

	shader_ptr s = shader_ptr(new Shader("../shaders/vshader.glsl", "../shaders/fshader.glsl"));
	State state = State();
	state.set(State::Attribute::SHADER,s);
	grp->setState(&state);

	return grp;
*/

}

int main( void )
{
	//MainWindow mainWindow = MainWindow();
	MainWindow::getInstance().init(1024,768);
	//mainWindow.init(1024,768);

	group_ptr grp = build_graph();
	RenderVisitor r = RenderVisitor();	
	UpdateVisitor u = UpdateVisitor();	

	//while(mainWindow.isRunning()){
	while(MainWindow::getInstance().isRunning()){
		MainWindow::getInstance().clear();
		MainWindow::getInstance().getInput();
		MainWindow::getInstance().update();	

	//	std::cout << "NEW FRAME" << std::endl;
		u.traverse(grp.get());
		r.traverse(grp.get());
	
		// Swap buffers
		MainWindow::getInstance().swap();
	} 

	MainWindow::getInstance().destroy();
	return 0;
}
