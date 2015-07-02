#include "CollisionPlane.hpp"
#include "BoundingSphere.hpp"
CollisionPlane::CollisionPlane()
{
	point << 0.0f, 0.0f, 0.0f;
	planeNormal << 0.0f, 1.0f, 0.0f;
}

CollisionPlane::~CollisionPlane()
{
}

// Point
bool CollisionPlane::intersect(Eigen::Vector3d p)
{
	// Vector pointing from the plane to the point p
	Eigen::Vector3d dp =  p - this->point;
	
	double dot_p = this->planeNormal.dot(dp);
	
	if(dot_p >= 0)
	{
		return false;
	}
	return true;
}

// Plane
bool CollisionPlane::intersect(CollisionPlane* cp)
{
	// We never use plane plane intersection, it's not even clear what that means
	// in the context we are using. But lets use the mathematical definition, even 
	// though its useless for us.
	Eigen::Vector3d dn = this->planeNormal.cross(cp->planeNormal);
	if(dn.norm() < 0.01)
	{
		return false;
	}

	return true;
}

// Sphere
bool CollisionPlane::intersect(BoundingSphere* bs)
{
	Eigen::Vector3d dr = point - bs->_o;

	// Length from centre of sphere to plane normal
	double len = planeNormal.dot(dr);

	if(len >= bs->_r)
	{
		return false;
	}
	
	return true;
}

void CollisionPlane::transform(mat4 t)
{
	vec4 p = vec4(point(0), point(1), point(2), 1);
	p = t*p;
	point << p[0], p[1], p[2];

	vec4 n = vec4(point(0), point(1), point(2), 0);
	p = t*n;
	planeNormal << n[0], n[1], n[2];
}
