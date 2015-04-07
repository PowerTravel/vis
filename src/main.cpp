#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include "MainWindow.hpp"

#include "Shader.hpp"

#include "ParticleSystem.hpp"
#include "Geometry.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "RenderVisitor.hpp"
#include "UpdateVisitor.hpp"
#include "State.hpp"

#include "CameraMovementCallback.hpp"

#include <memory>


group_ptr build_graph();

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

//	p.printToFile("bajsloek.m");

	MainWindow::getInstance().destroy();

	return 0;
}

group_ptr build_graph()
{
	// Create shader state
	shader_ptr s = shader_ptr(new Shader("../shaders/phong_vshader.glsl", "../shaders/phong_fshader.glsl"));
	s->createUniform("Diff");
	s->createUniform("Amb");
	s->createUniform("Spec");
	s->createUniform("lPos");
	s->createUniform("att");
	s->createUniform("shi");

	State state = State();
	state.set(State::Attribute::SHADER,s);

	// Create Nodes
	// Root
	group_ptr grp = group_ptr(new Group());
	grp->setState(&state);
	// Camera
	camera_ptr cam = camera_ptr(new Camera());
	cam->connectCallback(callback_ptr(new CameraMovementCallback(cam)));
	// Transform 1 and 2
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
	
	// ParticleSystem
	partsys_ptr ps = partsys_ptr(new ParticleSystem);

	// Geometry
	geometry_vec gvec = Geometry::loadFile("../models/sphere.obj");


	// Link the tree

	grp->addChild(cam);

	cam->addChild(trns1);
	cam->addChild(trns2);
	cam->addChild(ps);

	for(int i = 0; i<gvec.size(); i++){
		trns1->addChild(gvec[i]);
		trns2->addChild(gvec[i]);
	}

	return grp;
}

