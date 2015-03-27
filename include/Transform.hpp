#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Group.hpp"

#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp> // from mat4 to float[]
//#include <glm/ext.hpp>		// from mat4 to string

class NodeVisitor;

#ifndef _TRANSFORM_PTR
#define _TRANSFORM_PTR
class Transform;
typedef std::shared_ptr<Transform> transform_ptr;
#endif // _TRANSFORM_PTR

#ifndef _VEC3
#define _VEC3
typedef glm::vec3 vec3;
#endif // _VEC3

#ifndef _VEC4
#define _VEC4
typedef glm::vec4 vec4;
#endif // _VEC4

#ifndef _MAT4
#define _MAT4
typedef glm::mat4 mat4;
#endif // MAT4

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
