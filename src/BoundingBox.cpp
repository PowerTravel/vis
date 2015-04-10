#include "BoundingBox.hpp"
#include "Eigen/Eigenvalues"
#include "Eigen/Dense"
#include <iostream>

BoundingBox::BoundingBox()
{
	_mean = Eigen::Vector3d::Zero();
	_x = Eigen::Vector3d::Zero();
	_y = Eigen::Vector3d::Zero();
	_z = Eigen::Vector3d::Zero();
}
// int n is the number of points, 
// double* points is a 3*n long array ordered  x0,y0,z0,x1,y1,z1,... etc
BoundingBox::BoundingBox(int n, float* points)
{
	build(n, points);
}
BoundingBox::~BoundingBox()
{

}


void BoundingBox::build(int n, float* points)
{
	Eigen::Map<Eigen::VectorXf >  tmp(points,3*n);
	Eigen::VectorXd pts = tmp.cast<double>();
	Eigen::Vector3d mean = Eigen::Vector3d::Zero();
	for(int i = 0; i<n; i++)
	{
		mean += pts.segment(3*i, 3); 
	}
	mean = mean/n;

	Eigen::Matrix3d covMat;

	// Find the coordinate system that is aligned along the major 
	// axis of the object.
	for(int i = 0; i<3; i++)
	{
		for(int j = 0; j<3; j++)
		{
			covMat(i,j) = 0.0f;
			for(int k = 0; k<n; k++)
			{
				Eigen::Vector3d p = pts.segment(3*k, 3);
				covMat(i,j) += (mean(i) - p(i)) * (mean(j) - p(j));
			}
			covMat(i,j) /= n-1;
		}	
	}

	Eigen::EigenSolver<Eigen::Matrix3d> es;
	es.compute(covMat, true);

	Eigen::Matrix3d coord_sys;
	coord_sys <<	es.eigenvectors().real().col(0),
					es.eigenvectors().real().col(1),
					es.eigenvectors().real().col(2);
	
	//Scale the axis to fit the span of the object
	//Eigen::Vector3d diff = pts.segment(0,3) - mean;
	Eigen::Vector3d p = pts.segment(0,3);
	Eigen::Vector3d diff = p - mean;
	Eigen::Vector3d max;
	max << 	p.transpose() * coord_sys.col(0),
			p.transpose() * coord_sys.col(1),
			p.transpose() * coord_sys.col(2);
	for( int i = 1; i<n; ++i )
	{
		p = pts.segment(3*i,3);
		diff = p - mean;
		for( int j = 0; j<3; ++j )
		{
			double dot = diff.transpose() * coord_sys.col(j);
			if( dot > max(j) )
			{
				max(j) = dot;	
			}
		}
	}

	_mean = mean;
	_x = max(0) * coord_sys.col(0);
	_y = max(1) * coord_sys.col(1);
	_z = max(2) * coord_sys.col(2);
}

void BoundingBox::build(int n, BoundingBox* bbList)
{
	// Get corners from all the BBes in the list and construct a new BB
}

bool BoundingBox::intersect(BoundingBox* box_ptr)
{
	// Check if this intersects box_ptr
	return false;
}

bool BoundingBox::intersect(BoundingBox* box_ptr1, BoundingBox* box_ptr2)
{
	// Check if the two boxes intersect
	return false;
}
		
void BoundingBox::getCorners(double* corners)
{

}

