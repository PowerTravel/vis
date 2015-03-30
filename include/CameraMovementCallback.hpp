#ifndef CAMERA_MOVEMENT_CALLBACK_HPP
#define CAMERA_MOVEMENT_CALLBACK_HPP
#include "Camera.hpp"


/*	
 *	Class: 		CameraMovementCallback
 *	Purpose: 	Controlls the camera movement
 *	Misc:		-
 */
class CameraMovementCallback : public UpdateCallback
{
	public:
		CameraMovementCallback(camera_ptr cam);
		virtual ~CameraMovementCallback();

		void execute();
		void getKeyState();

	private:
		camera_ptr _cam;
		float _speed;
		float _angularVelocity;
		
};

#endif //CAMERA_MOVEMENT_CALLBACK_HPP
