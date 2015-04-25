#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#include "Matlib.hpp"
#include <vector>
#include <iostream>

class Group;

class BoundingBox
{
	public:
		BoundingBox();
		BoundingBox(int n, float* points);
		BoundingBox(int n, double* points);
		virtual ~BoundingBox();

		void getCorners(double* p);
		double width();
		double depth();
		double height();
		void transform(mat4 t);

		bool zombie();
	
		// see if the coodinate x,y,z is contained within the bb.
		bool contain(double x, double y, double z);
		bool contain(Eigen::Vector3d v);

		bool intersect(BoundingBox* box_ptr); // Not implemented
		static bool intersect(BoundingBox* box_ptr1, BoundingBox* box_ptr2); // Not implemented

		void print();
	private:
		//The center is the average of the points and the box are the eigenvectors of the covariance matrix	
		Eigen::Matrix3d _coord_sys; // Columns are unit vectors of the aligned box
		Eigen::Vector3d	_max; 		// How much we should scale each unit coord to get the full dimension of the box
		// Example: x_max = _coord_sys.col(1) * max(0) is the length of the box from its center to the edge in its x-direction.
		// Same goes for y, and z;
		Eigen::Vector3d _mean; // The center of the box.

		std::vector<vec4> _corners;

		bool _initiated;

		void build(int n, Eigen::VectorXd& points);
		void setCorners();
		
};

#endif // BOUNDING_BOX_HPP
