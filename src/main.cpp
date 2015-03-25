//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <cstdio>
#include <stdlib.h>
#include "MainWindow.hpp"
#include "gmtl/gmtl.h"
//#include <assimp/Importer.hpp>
//#include <assimp/postprocess.h>
//#include <glm/vec2.hpp>

//void glfw_error_callback(int error, const char* error_message)
//{
//	fprintf(stderr, "%s\n", error_message);
//}

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
	glClearColor(0.0,0.0,0.0,0.0);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glFrontFace(GL_CW);
}

int main(int argc, char* argv[])
{
	MainWindow window = MainWindow();
	setUpGlew();

	while(window.isRunning()){
		window.getInput();
		window.update();

		window.clear();
		window.swap();
	} 	return 0;
}
