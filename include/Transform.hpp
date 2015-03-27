#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Group.hpp"
#include "MatLib.hpp"

//#include <glm/gtc/type_ptr.hpp> // from mat4 to float[]
//#include <glm/ext.hpp>		// from mat4 to string

#ifndef _TRANSFORM_PTR
#define _TRANSFORM_PTR
class Transform;
typedef std::shared_ptr<Transform> transform_ptr;
#endif // _TRANSFORM_PTR



class Transform: public Group
{
	public:
		Transform();
		virtual ~Transform();

		mat4 get();
		void set(mat4 m);

		void translate(vec3 ds);
		void rotate(float angle, vec3 axis);
		void scale(vec3 ds);

		void acceptVisitor(NodeVisitor& visitor);

	private:
		mat4 _m;
};

#endif // TRANSFORM_HPP
