#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#include "Matlib.hpp"
#include "Eigen/Dense"

// Bare minimum implementation of BoundingBox to work with a flat floor surface, Interface is general
// The bounding box of a geometry are in object space
// The other bounding boxes are in world-space


// DOES NOT WORK FOR SPHERES, MAY BE THAT WE GET DUPLICATE POINTS.
class BoundingBox
{
	public:
		BoundingBox();
		BoundingBox(int n, float* points);
		virtual ~BoundingBox();

		bool intersect(BoundingBox* box_ptr); // Not implemented
		static bool intersect(BoundingBox* box_ptr1, BoundingBox* box_ptr2); // Not implemented

	private:
		//The center is the average of the points and the box are the eigenvectors of the covariance matrix	
		Eigen::Vector3d _mean;
		Eigen::Vector3d _x;
		Eigen::Vector3d _y;
		Eigen::Vector3d _z;

		void build(int n, float* points);
		void build(int n, BoundingBox* bbList); // Not Implemented
		
		void getCorners(double* corners); // Not implemented
};

#endif // BOUNDING_BOX_HPP
