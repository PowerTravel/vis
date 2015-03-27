#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <iostream>

#include "Scene.hpp"
#include "RenderVisitor.hpp"
#include "UpdateVisitor.hpp"
#include "KeyState.hpp"
#include "Timer.hpp"



void setUpGlut(int argc, char* argv[]);
void setUpGlew();
void glutMainProgram();
void GLEWprintSystemSpecs();

// A few callbacks
void updateAndDisplay(int i);
void display();
void idleFunk();

// A few global variables.
int SCREEN_WIDTH = 512;
int SCREEN_HEIGHT = 512;
int FPS = 60;
//UpdateVisitor u;
//RenderVisitor r;	 
// used to calculate fps and scenegraph traverse time.
float AVERAGE_FPS = 0;
int TICKS =0;
Timer FPS_TIMER;
Timer RENDER_TIMER;

int main(int argc, char* argv[])
{
	// Set up glut and glew
	setUpGlut(argc, argv);
	setUpGlew();
//	r = RenderVisitor();

	// Build our scene
	Scene::getInstance().buildScene(Scene::LAB2);

	// KeyState records buttonpresses
	KeyState::getInstance().setGlutCallback();

	// Run the main program
	glutMainProgram();	

	return 0;
}

void updateAndDisplay(int i)
{	
	// Calculates the average fps over 60 frames
	AVERAGE_FPS += FPS_TIMER.getTime();
	// Calcuates the time spent traversing the scene graph
	RENDER_TIMER.start(); 

	// Exit program if user press 'q'
	if( KeyState::getInstance().get() & key_state_bit::KEY_Q)
	{
		glutLeaveMainLoop();
	}

	// let the updateVisitor traverse the scenegraph	
	Scene::getInstance().getUpdateVisitor()->traverse(Scene::getInstance().getRoot());
	// Among other things lets the renderVisitor traverse the scenegraph
	display();

	// Calculate the average fps over FPS frames
	if(TICKS == FPS)
	{
		std::string title = "Assignment 1, FPS: ";
		title.append(std::to_string(FPS*AVERAGE_FPS));
		title.append("  SceneGraphTime [s]: ");
		title.append(std::to_string( RENDER_TIMER.getTime()));
		glutSetWindowTitle(title.c_str());
		AVERAGE_FPS = 0;
		TICKS=0;
	}

	// Call this function again in 1/FPS seconds
	glutTimerFunc(1000/FPS, updateAndDisplay,0);

	RENDER_TIMER.start();
	FPS_TIMER.start();
	TICKS++;
}

void display()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Traverse the scenegraph and draw all objects
	Scene::getInstance().getRenderVisitor()->traverse(Scene::getInstance().getRoot());
	//r.traverse(Scene::getInstance().getRoot());

	glutSwapBuffers();
}


// Below are general setup calls to glut and glew.
void setUpGlut(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	glutInitContextVersion(3,3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(50,50);
	glutCreateWindow("Assignment 1");
	glViewport(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
}

void idleFunk()
{

}

void glutMainProgram()
{
	//glutSetCursor(GLUT_CURSOR_NONE);
	glutDisplayFunc(display);

	glutIgnoreKeyRepeat(1);

	glutIdleFunc(idleFunk);

	updateAndDisplay(1);
	glutMainLoop();
}

/*
void GLEWprintSystemSpecs()
{
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* shading = glGetString(GL_SHADING_LANGUAGE_VERSION);
	
	GLint numExt;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);

	GLint numLang; 
	glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &numLang);
	const GLubyte* extension = glGetStringi(GL_EXTENSIONS, numExt-1 );
	const GLubyte* shadeVers = glGetStringi(GL_SHADING_LANGUAGE_VERSION, numLang-1 );

	fprintf(stderr, "\n -=System Specs=- \n");
	fprintf(stderr, "vendor = %s \n", vendor);
	fprintf(stderr, "renderer = %s \n", renderer);
	fprintf(stderr, "GL Version = %s \n", version);
	fprintf(stderr, "Shading language version = %s \n", shading);
	fprintf(stderr, "shading extenstion = %s \n\n", extension);
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}
*/

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
/*
	if(	glewGetExtension("GL_ARB_fragment_shader") != GL_TRUE ||
		glewGetExtension("GL_ARB_vertex_shader") != GL_TRUE ||
		glewGetExtension("GL_ARB_shader_objects") != GL_TRUE ||
		glewGetExtension("GL_ARB_shading_language_100") != GL_TRUE  )
	{
		std::cerr << "Driver does not support OpenGL shading language" << std::endl;
		exit(1);
	}
*/

	// this prints the computers compatibility.
	// Does not work on itchy
	 //GLEWprintSystemSpecs();	
}
