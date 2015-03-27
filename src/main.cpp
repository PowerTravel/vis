#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include "MainWindow.hpp"

#include "Shader.hpp"

#include "Geometry.hpp"
#include "Group.hpp"
#include "Transform.hpp"
#include "NodeVisitor.hpp"

#include <memory>

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
	glClearColor(0.0,0.0,0.0,0.0);


	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	//glFrontFace(GL_CW);
}

group_ptr build_graph()
{
	geometry_vec gvec = Geometry::loadFile("../models/box.obj");
	group_ptr grp = group_ptr(new Group());
	transform_ptr trns1 = transform_ptr(new Transform());
	trns1->translate(vec3(0.5,0,0));
	transform_ptr trns2 = transform_ptr(new Transform());
	trns2->translate(vec3(-0.5,0,0));

	for(int i = 0; i<gvec.size(); i++){
		trns1->addChild(gvec[i]);
		trns2->addChild(gvec[i]);
	}

	grp->addChild(trns1);
	grp->addChild(trns2);

	return grp;
}

int main(int argc, char* argv[])
{
	MainWindow window = MainWindow();
	setUpGlew();

	group_ptr graph = build_graph();
	Shader s = Shader("../shaders/vshader.glsl", "../shaders/fshader.glsl");
	NodeVisitor n = NodeVisitor();

	std::cout<< glm::to_string(glm::mat4(1.0f)) << std::endl;
	

	while(window.isRunning()){
		window.clear();

		window.getInput();
		window.update();

	//	std::cout << "New Frame "<<std::endl;		
	//	n.traverse(graph.get());
		

		window.swap();
	} 	

	return 0;
}
