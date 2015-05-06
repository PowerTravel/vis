#include "BoundingBox.hpp"
#include "Eigen/Eigenvalues"
#include "Eigen/Dense"
#include <iostream>

BoundingBox::BoundingBox()
{
	_initiated = false;
}

// int n is the number of points, 
// double* points is a 3*n long array ordered  x0,y0,z0,x1,y1,z1,... etc
BoundingBox::BoundingBox(int n, float* points)
{
	Eigen::Map<Eigen::VectorXf >  tmp(points,3*n);
	Eigen::VectorXd pts = tmp.cast<double>();
	build(n, pts);
	_initiated = true;
}

BoundingBox::BoundingBox(int n, double* points)
{
	Eigen::Map<Eigen::VectorXd >  tmp(points, 3*n);
	Eigen::VectorXd pts = tmp;
	build(n, pts);
	_initiated = true;
}

BoundingBox::~BoundingBox()
{
}

bool BoundingBox::zombie()
{
	return !_initiated;
}

void BoundingBox::build(int n, Eigen::VectorXd& pts)
{
	_coord_sys = Eigen::Matrix3d::Identity();
	_max = Eigen::Vector3d::Zero();
	_mean = Eigen::Vector3d::Zero();

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

	
	_coord_sys = coord_sys;
	_mean = mean;
	_max = max;
	//std::cout<<_T.block(0,0,3,3) * _T.block(0,0,3,3).transpose()<< std::endl<<std::endl;

	setCorners();
}


void BoundingBox::setCorners()
{
	Eigen::Vector3d x = _max(0) * _coord_sys.col(0);
	Eigen::Vector3d y = _max(1) * _coord_sys.col(1);
	Eigen::Vector3d z = _max(2) * _coord_sys.col(2);
	int npv = 3; // Nr of numbers per Vertex
	double corners[24]; // 3*8 = 24; 8 corners, 3 numbers per corner
	for(int i = 0; i<npv; i++)
	{
		corners[0*npv + i] = _mean(i) + x(i) + y(i) + z(i);
		corners[1*npv + i] = _mean(i) + x(i) + y(i) - z(i);
		corners[2*npv + i] = _mean(i) + x(i) - y(i) + z(i);
		corners[3*npv + i] = _mean(i) + x(i) - y(i) - z(i);
		corners[4*npv + i] = _mean(i) - x(i) + y(i) + z(i);
		corners[5*npv + i] = _mean(i) - x(i) + y(i) - z(i);
		corners[6*npv + i] = _mean(i) - x(i) - y(i) + z(i);
		corners[7*npv + i] = _mean(i) - x(i) - y(i) - z(i);
	}

	_corners = std::vector<vec4>(8);
	for(int i=0; i<8; i++)
	{
		vec4 v = vec4(corners[3*i+0],corners[3*i+1],corners[3*i+2], 1.0);
		_corners[i] = v;
	}
}
/*
void BoundingBox::transform(mat4 t)
{
	mat4 trans_mat = mat4(1.0);
	glm::column(trans_mat, 3, glm::column(t,3) );
	vec4 mean = vec4(_mean(0), _mean(1), _mean(2), 1);
	mean = t*mean;
	_mean << mean[0], mean[1], mean[2];

	for(int i = 0; i<8; i++)
	{
		_corners[i] = t*_corners[i];
	}


	_max(0) = width()/2;
	_max(1) = height()/2;
	_max(2) = depth()/2;
}
*/

void BoundingBox::transform(mat4 t)
{
	if(_initiated)
	{
		Eigen::VectorXd vp = Eigen::VectorXd(24);	
		for(int i = 0; i<8; i++)
		{
			vec4 v = t*_corners[i];
			vp.segment(3*i, 3) << v[0],v[1],v[2];
		}
		build(8, vp);
	}
}

void BoundingBox::getCorners(double* p)
{
	if( _initiated)
	{
		for(int i = 0; i<8 ; i++)
		{
			vec4 v = _corners[i];
			p[3*i+0] = v[0];
			p[3*i+1] = v[1];
			p[3*i+2] = v[2];
		}
	}
}

bool BoundingBox::contain(double x, double y, double z)
{
	return contain(Eigen::Vector3d(x,y,z));
}

bool BoundingBox::contain(Eigen::Vector3d v)
{
	// Change from woorld coordinates to box-coordinates
	Eigen::Vector3d vp = _coord_sys.transpose() * v;	

	// Find the range along the different axis
	double x_min = _mean(0) - _max(0);
	double x_max = _mean(0) + _max(0);
	double y_min = _mean(1) - _max(1);
	double y_max = _mean(1) + _max(1);
	double z_min = _mean(2) - _max(2);
	double z_max = _mean(2) + _max(2);

	//std::cout << v.transpose() << std::endl; 
	//std::cout << "BBContain xmin,xmax "<< x_min << " " << x_max << std::endl;
	//std::cout << "BBContain ymin,ymax "<< y_min << " " << y_max << std::endl;
	//std::cout << "BBContain zmin,zmax "<< z_min << " " << z_max << std::endl;

	// If any of the coordinates are outside the bounds
	// of the box we return false
	if( (vp(0) <= x_min) || ( vp(0) >= x_max ) ||
		(vp(1) <= y_min) || ( vp(1) >= y_max ) ||
		(vp(2) <= z_min) || ( vp(2) >= z_max ) )
	{
		return false;
	}
//	print();
	return true;
}
	//#include <glm/gtx/norm>	
double BoundingBox::width()
{
	if(_initiated)
	{
		// x-dir
		vec4 p62 = _corners[2] - _corners[6];
		//vec4 p40 = _corners[0] - _corners[4];
		//vec4 p73 = _corners[3] - _corners[7];
		//vec4 p51 = _corners[1] - _corners[5];
		
		return glm::length(p62);
	}
	return -1;
}

double BoundingBox::depth()
{
	if(_initiated)
	{
		// z-dir
		vec4 p01 = _corners[1] - _corners[0];
	//	vec4 p23 = _corners[3] - _corners[2];
	//	vec4 p67 = _corners[7] - _corners[6];
	//	vec4 p45 = _corners[5] - _corners[4];

		return glm::length(p01);
	}
	return -1;
}
double BoundingBox::height()
{
	if(_initiated)
	{
		// y-dir
		vec4 p20 = _corners[0] - _corners[2];
		//vec4 p31 = _corners[1] - _corners[3];
		//vec4 p64 = _corners[4] - _corners[6];
		//vec4 p75 = _corners[5] - _corners[7];
		
		return glm::length(p20);
	}
	return -1;
}

void BoundingBox::print()
{
	if(_initiated)
	{
		std::cout << "BoundingBox::print()." << std::endl;
		for(int i = 0; i<8; i++)
		{
			std::cout << glm::to_string(_corners[i]) << std::endl;
		}
	}else{
		std::cerr << "BoundingBox::print() -  not initiated." << std::endl;
	}	
}

/*
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
*/
