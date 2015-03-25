#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "Group.hpp"
#include "Hmat.hpp"
#include "TransformMatrix.hpp"

#ifndef CAMERA_PTR
#define CAMRA_PTR
class Camera;
typedef std::shared_ptr<Camera> camera_ptr;
#endif //CAMERA_PTR

/*
 *	Class: 		Camera
 *	Purpose: 	A camera.
 *	Misc:		Derived from group.
 */
class Camera : public Group
{

	public:
		Camera();
		Camera(Vec3 eye, Vec3 at, Vec3 up);
		virtual ~Camera();

		// Take a visitor
		void acceptVisitor(NodeVisitor& v);

		// Manipulation of position and orientation
		void lookAt(Vec3 v);
		void lookAt(Vec3 eye, Vec3 at, Vec3 up);
		void rotate(float angle, Vec3 axis);
		void translate(Vec3 t);

		// Getters of the view and projection matrices
		Hmat getProjectionMat();
		Hmat getViewMat();
		Vec3 getPos();

		// Support is given for perspective and orthographic projection
		void setPerspectiveProjection(float fovy=35.f, float near=1.f, float far=10);
		void setOrthographicProjection(float left=-10.f, float right=10.f, float bottom=-10.f, float top=10.f, float near = 10.f, float far =-10.f );

		void setAspect(float aspect);

		void update();
	private:
	
		Hmat _P; // Projection matrix
		TransformMatrix _V; // View matrix
		Hmat _R;

		// Variables related to perspective and orthographic projection
		float _fovy, _aspect, _near, _far;

		bool _moved, _perspectiveChange;
		
		// Variables related to position and orientation in World Coord
		Vec3 _pos, _x, _y, _z;
		// The actual implementation of perspective and orthographic projection
		void perspective();
		void orthographic( float left, float right, float bottom,
				    float top,  float near,  float far);

		void updatePosition();
};

#endif // CAMERA_HPP
