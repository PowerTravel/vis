#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#include "Matlib.hpp"
#include "Eigen/Dense"


class Group;

class BoundingBox
{
	public:
		BoundingBox();
		BoundingBox(int n, float* points);
		BoundingBox(int n, double* points);
		virtual ~BoundingBox();

		void getCorners(double* corners);
		//void get(double* center, double* x, double* y, double* z);

		bool intersect(BoundingBox* box_ptr); // Not implemented
		static bool intersect(BoundingBox* box_ptr1, BoundingBox* box_ptr2); // Not implemented

	private:
		//The center is the average of the points and the box are the eigenvectors of the covariance matrix	
		Eigen::Vector3d _mean;
		Eigen::Vector3d _x;
		Eigen::Vector3d _y;
		Eigen::Vector3d _z;
		bool _initiated;

		void build(int n, Eigen::VectorXd& points);
		
};

#endif // BOUNDING_BOX_HPP
