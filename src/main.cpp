#include "MainWindow.hpp"

//#include "LocalUpdateVisitor.hpp"
//#include "GlobalUpdateVisitor.hpp"

#include "Group.hpp"
#include "Camera.hpp"
#include "Transform.hpp"
#include "Geometry.hpp"
#include "GraphVisitor.hpp"
#include "CameraMovementCallback.hpp"
#include "RotTransCallback.hpp"
#include "ParticleSystem.hpp"
//#include "Transform.hpp"


//#include "PhysicsVisitor.hpp"
//#include "ParticleSystem.hpp"

//#include "CameraMovementCallback.hpp"
//#include "RotTransCallback.hpp"

//#include "CollisionEngine.hpp"

group_ptr build_graph();
group_ptr build_graph_simple();
group_ptr build_graph_branch();

int main( void )
{
	MainWindow::getInstance().init(1024,768);

	group_ptr grp = build_graph();

	GraphVisitor gv = GraphVisitor();

//	gv.traverse(grp.get());

//	group_ptr grp = build_graph();

//	LocalUpdateVisitor lu = LocalUpdateVisitor();			// Visits and make LOCAL changes to the nodes
//	GlobalUpdateVisitor gu = GlobalUpdateVisitor();			// Visits and make GLOBAL changes to the nodes
	
	RenderList* rl = gv.getRenderList(); 					// A list of renderNodes that is static member of NodeVisitor and all derived classes.
	
	//PhysicsVisitor fv = PhysicsVisitor();					// Updates physics related nodes

//	CollisionEngine cle = CollisionEngine(rl);

	while(MainWindow::getInstance().isRunning())
	{
		MainWindow::getInstance().clear();
		MainWindow::getInstance().getInput();
		MainWindow::getInstance().update();	

		//std::cout << "NEW FRAME" << std::endl;	
		//lu.traverse(grp.get());
		//gu.traverse(grp.get());
		//fv.traverse();

		//cle.update();

		gv.traverse(grp.get());
		rl->draw();

		MainWindow::getInstance().swap();
	} 
	
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

	// Spheres
	transform_ptr sphere_rot = transform_ptr(new Transform());
	sphere_rot->connectCallback(callback_ptr(new RotTransCallback(sphere_rot, 0.02)));

	state = State();
	state.set(State::Attribute::RENDER_MODE, State::Value::LINE);
	state.set(State::Attribute::BACK_FACE_CULLING, State::Value::OFF);

	transform_ptr sphere1 = transform_ptr(new Transform());
	sphere1->translate(vec3(-2,0,0));
	sphere1->scale(vec3(2,2,2));
	sphere1->setState(&state);

	transform_ptr sphere2 = transform_ptr(new Transform());
	sphere2->translate(vec3(2,0,0));
	sphere2->scale(vec3(2,2,2));

	transform_ptr sphere_spin  = transform_ptr(new Transform());
	sphere_spin->connectCallback(callback_ptr(new RotTransCallback(sphere_spin, 1)));

	// Floor
	transform_ptr floor = transform_ptr(new Transform());
	floor->translate(vec3(0,-4,0));
	floor->scale(vec3(50,0.1,50));

	// ParticleSystem
	partsys_ptr ps = partsys_ptr(new ParticleSystem);

	// Geometry
	geometry_vec m_sphere = Geometry::loadFile("../models/sphere.obj");
	geometry_vec m_box = Geometry::loadFile("../models/box.obj");

	// Link the tree
	grp->addChild(cam);
	cam->addChild(sphere_rot);
	
	sphere_rot -> addChild(sphere1);
	sphere_rot -> addChild(sphere2);

	sphere1 -> addChild(sphere_spin);
	sphere2 -> addChild(sphere_spin);

	cam->addChild(floor);
	cam->addChild(ps);

	for(int i = 0; i<m_sphere.size(); i++){
		sphere_spin->addChild(m_sphere[i]);
	}
	

	for(int i = 0; i<m_box.size(); i++){
		floor->addChild(m_box[i]);
	}

	return grp;
}

group_ptr build_graph_branch()
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

	// Spheres
	transform_ptr sphere_rot = transform_ptr(new Transform());
	sphere_rot->connectCallback(callback_ptr(new RotTransCallback(sphere_rot, 0.02)));

	state = State();
	state.set(State::Attribute::RENDER_MODE, State::Value::LINE);
	state.set(State::Attribute::BACK_FACE_CULLING, State::Value::OFF);

	transform_ptr sphere1 = transform_ptr(new Transform());
	sphere1->translate(vec3(-2,0,0));
	sphere1->scale(vec3(2,2,2));
	sphere1->setState(&state);

	transform_ptr sphere2 = transform_ptr(new Transform());
	sphere2->translate(vec3(2,0,0));
	sphere2->scale(vec3(2,2,2));

	transform_ptr sphere_spin  = transform_ptr(new Transform());
	sphere_spin->connectCallback(callback_ptr(new RotTransCallback(sphere_spin, 1)));

	// Floor
	transform_ptr floor = transform_ptr(new Transform());
	floor->translate(vec3(0,-4,0));
	floor->scale(vec3(50,0.1,50));

	// Geometry
	geometry_vec m_sphere = Geometry::loadFile("../models/sphere.obj");
	geometry_vec m_box = Geometry::loadFile("../models/box.obj");

	// Link the tree
	grp->addChild(cam);
	cam->addChild(sphere_rot);
	cam->addChild(floor);
	
	sphere_rot -> addChild(sphere1);
	sphere_rot -> addChild(sphere2);

	sphere1 -> addChild(sphere_spin);
	sphere2 -> addChild(sphere_spin);

	for(int i = 0; i<m_sphere.size(); i++){
		sphere_spin->addChild(m_sphere[i]);
	}
	
	for(int i = 0; i<m_box.size(); i++){
		floor->addChild(m_box[i]);
	}
	
	return grp;
}

group_ptr build_graph_simple()
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

	// Floor
	transform_ptr floor = transform_ptr(new Transform());
	floor->translate(vec3(0,-4,0));
	floor->scale(vec3(50,0.1,50));
	floor->connectCallback(callback_ptr(new RotTransCallback(floor, 0.002)));

	// Geometry
	geometry_vec m_box = Geometry::loadFile("../models/box.obj");

	// Link the tree

	grp->addChild(cam);
	cam->addChild(floor);

	for(int i = 0; i<m_box.size(); i++){
		floor->addChild(m_box[i]);
	}

	return grp;
}
