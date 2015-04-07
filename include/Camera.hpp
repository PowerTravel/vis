#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Group.hpp"
#include "MatLib.hpp" // Inclusion and Typedefs of glm library

#ifndef _CAMERA_PTR
#define _CAMRA_PTR
class Camera;
typedef std::shared_ptr<Camera> camera_ptr;
#endif //CAMERA_PTR


/*
 *	Class: 		Camera
 *	Purpose: 	A camera group node.
 *	Misc:		Derived from group.
 */
class Camera : public Group
{

	public:
		Camera();
		Camera(vec3 eye, vec3 at, vec3 up);
		virtual ~Camera();

		// Take a visitor
		void acceptVisitor(NodeVisitor& v);

		// Manipulation of position and orientation
		void lookAt(vec3 eye, vec3 at, vec3 up);
		void rotate(float angle, vec3 axis);
		void translate(vec3 t);

		// Getters of the view and projection matrices
		mat4 getProjectionMat();
		mat4 getViewMat();

		// Support is given for perspective and orthographic projection
		void setPerspectiveProjection(float fovy=45.f, float aspect=4.0f/3.0f, float near=0.1f, float far=500);
		void setOrthographicProjection(float left=-10.f, float right=10.f, float bottom=-10.f, float top=10.f, float near = 10.f, float far =-10.f );

		void update();
	private:
	
		mat4 _P; // Projection matrix
		mat4 _V; // View matrix

		vec3 _pos;
		vec3 _at;

		vec3 _dr;
		mat4 _dw;
		bool _moved, _projectionChange;

		mat4 getRigidInv();
};

#endif // CAMERA_HPP

