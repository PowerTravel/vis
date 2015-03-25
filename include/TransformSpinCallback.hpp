#ifndef TRANSFORM_SPIN_CALLBACK 
#define TRANSFORM_SPIN_CALLBACK 

#include "UpdateCallback.hpp"
#include "Vec3.hpp"

#ifndef TRANSFORM_PTR
#define TRANSFORM_PTR
class Transform;
typedef std::shared_ptr<Transform> transform_ptr;
#endif // TRANSFORM_PTR


/*	
 *	Class: 		TransformSpinCallback
 *	Purpose: 	Adds a rotation to a Transform node
 *	Misc:		-
 */
class TransformSpinCallback : public UpdateCallback
{
	public:

		TransformSpinCallback(transform_ptr t);
		TransformSpinCallback(transform_ptr t, float angle, Vec3 axis);
		virtual ~TransformSpinCallback();
	
		void execute();

	private:
		transform_ptr _t;
		float _angle; // The angular velocity
		Vec3 _axis;	// The axis of rotation
};

#endif // TRANSFORM_SPIN_CALLBACK 
