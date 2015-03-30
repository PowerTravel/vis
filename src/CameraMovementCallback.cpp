#include "CameraMovementCallback.hpp"
#include <iostream>
#include "Scene.hpp"
CameraMovementCallback::CameraMovementCallback(camera_ptr cam)
{	
	_cam = cam;
	_angularVelocity = 0.02;
	_speed = 0.1;
}

CameraMovementCallback::~CameraMovementCallback()
{

}

void CameraMovementCallback::execute()
{
	//getKeyState();
}

// Reads the keystate from the keyState singleton class
// and updates the camera
void CameraMovementCallback::getKeyState()
{	
	
	if( st & key_state_bit::KEY_W) 
	{
		_cam->translate(Vec3(0,0,-_speed));
	}
	if( st & key_state_bit::KEY_A)
	{
		_cam->translate(Vec3(-_speed,0,0));
	}
	if( st & key_state_bit::KEY_S )
	{
		_cam->translate(Vec3(0,0,_speed));
	}
	if( st & key_state_bit::KEY_D )
	{
		_cam->translate( Vec3(_speed,0,0));
	}

	if( st & key_state_bit::KEY_Z )
	{
		_cam->translate(Vec3(0,-_speed,0));

	}
	if( st & key_state_bit::KEY_X )
	{
		_cam->translate(Vec3(0,_speed,0));

	}
	if( st & key_state_bit::KEY_R)
	{
		_cam->lookAt(Vec3(0,0,4), Vec3(0,0,0), Vec3(0,1,0));
		_cam->setPerspectiveProjection();
	}
	if( st & key_state_bit::KEY_T)
	{
		_cam->lookAt(Vec3(0,5,4), Vec3(0,0,0), Vec3(0,1,0));
		_cam->setPerspectiveProjection();
	}
	
	KeyState::Special_Key_State sst = KeyState::getInstance().get_special();
	if( sst & special_key_state_bit::SPECIAL_KEY_UP )
	{
		_cam->rotate(_angularVelocity, Vec3(-1,0,0));
	}
	if( sst & special_key_state_bit::SPECIAL_KEY_DOWN )
	{
		_cam->rotate(_angularVelocity, Vec3(1,0,0));
	}
	if( sst & special_key_state_bit::SPECIAL_KEY_LEFT )
	{
		_cam->rotate(_angularVelocity, Vec3(0,-1,0));
	}
	if( sst & special_key_state_bit::SPECIAL_KEY_RIGHT )
	{
		_cam->rotate(_angularVelocity, Vec3(0,1,0));
	}

}

