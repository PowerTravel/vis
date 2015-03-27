#include "KeyState.hpp"
#include "Scene.hpp"
#include "UpdateVisitor.hpp"
#include <GL/freeglut.h>

KeyState::Key_State KeyState::_key = KeyState::Key_State::KEY_NULL;
KeyState::Special_Key_State KeyState::_special_key = KeyState::Special_Key_State::SPECIAL_KEY_NULL;
bool KeyState::mWindowChanged = false;

KeyState::KeyState()
{

}

KeyState::~KeyState()
{

}

KeyState& KeyState::getInstance()
{
	static KeyState instance;
	return instance;
}
KeyState::Key_State KeyState::get()
{	
	return _key;
}
KeyState::Special_Key_State KeyState::get_special()
{
	return _special_key;
}

void KeyState::setGlutCallback()
{
	glutKeyboardFunc(glut_key_callback);
	glutKeyboardUpFunc(glut_key_callback);
	glutSpecialFunc(glut_special_key_callback);
	glutSpecialUpFunc(glut_special_key_callback);
	glutReshapeFunc(resize_callback);
}

void KeyState::windowChanged()
{
	mWindowChanged=true;
}

bool KeyState::hasWindowChanged()
{
	bool c = false;
	if(mWindowChanged){
		mWindowChanged = false;
		c=true;
	}	
	return c;
}
void KeyState::set(int st)
{
	int currState = (int) _key;

	// Updates to the new state
	int newState =  currState ^ st; 

	_key = (Key_State) newState;
}
void KeyState::special_set(int st)
{
	int currState = (int) _special_key;
	
	int newState = currState ^ st; 
	
	_special_key = (Special_Key_State) newState;
}

void resize_callback(int width, int height)
{
	// this call to GL viewport should not be here.
	// It should be in lab1.cpp but I don't know how to make 
	// that work yet
	glViewport(0,0,width,height);
	KeyState::getInstance().windowChanged();
	Scene::getInstance().getUpdateVisitor()->windowChanged();	
}

void glut_key_callback(unsigned char c, int x, int y)
{
	int st = 0;
	if( c == 'a' || c == 'A' )
	{
		st  += (int) KeyState::KEY_A;	
	}
	if( c == 'd' || c == 'D' )
	{
		st  += (int) KeyState::KEY_D;	
	}
	if( c == 'o' || c == 'O' )
	{
		st  += (int) KeyState::KEY_O;	
	}
	if( c == 'p' || c == 'P' )
	{
		st  += (int) KeyState::KEY_P;	
	}
	if( c == 'q' || c == 'Q' )
	{
		st  += (int) KeyState::KEY_Q;	
	}
	if( c == 'r' || c == 'R')
	{
		st  += (int) KeyState::KEY_R;
	}
	if( c == 's' || c == 'S' )
	{
		st  += (int) KeyState::KEY_S;	
	}
	if( c == 't' || c == 'T' )
	{
		st  += (int) KeyState::KEY_T;	
	}
	if( c == 'w' || c == 'W' )
	{
		st += (int) KeyState::KEY_W;	
	}
	if( c == 'x' || c == 'X' )
	{
		st  += (int) KeyState::KEY_X;	
	}
	if( c == 'z' || c == 'Z' )
	{
		st  += (int) KeyState::KEY_Z;	
	}

	KeyState::getInstance().set( st );
}

void glut_special_key_callback(int c, int x, int y)
{
	int st = 0;
	if( c == GLUT_KEY_UP)
	{	
		st += (int) KeyState::SPECIAL_KEY_UP;
	}
	if( c == GLUT_KEY_DOWN)
	{
		st += (int) KeyState::SPECIAL_KEY_DOWN;
	}
	if( c == GLUT_KEY_LEFT)
	{
		st += (int) KeyState::SPECIAL_KEY_LEFT;
	}
	if( c == GLUT_KEY_RIGHT)
	{
		st += (int) KeyState::SPECIAL_KEY_RIGHT;
	}
	KeyState::getInstance().special_set( st );
}
