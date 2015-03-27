#ifndef TRANSFORM_HPP
#define TRANSOFRM_HPP

#include "Group.hpp"
#include "TransformMatrix.hpp"

class NodeVisitor;
class Hmat;
class Vec3;

#ifndef TRANSFORM_PTR
#define TRANSFORM_PTR
class Transform;
typedef std::shared_ptr<Transform> transform_ptr;
#endif // TRANSFORM_PTR

class Transform: public Group
{
	public:
		Transform();
		virtual ~Transform();

		Hmat getM();
		void setM(Hmat m);

		void rotate(float angle, Vec3 axis);
		void scale(Vec3 ds);
		void translate(Vec3 ds);

		void acceptVisitor(NodeVisitor& visitor);

	private:
		TransformMatrix _T;
		
};

#endif // TRANSFORM_HPP
