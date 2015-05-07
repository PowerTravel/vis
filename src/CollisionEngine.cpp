#include "CollisionEngine.hpp"
#include "Eigen/Dense"
#include <iostream>

#include "Debug.hpp"

CollisionEngine::CollisionEngine(RenderList* rl)
{
	_rList = rl;
	_max_size = 1000;
	_grid_size = 10;
	_grid = std::map<int, container>();
}

CollisionEngine::~CollisionEngine(){}

void CollisionEngine::update()
{
	if(_rList == NULL)
	{
		return;	
	}


	_grid = std::map<int, container>();
	_rList->first();
	do{
		BoundingBox b = _rList->get().getBoundingBox();
		if(b.zombie() )
		{
			break;
		}

		Eigen::VectorXd pts = Eigen::VectorXd(24);
		b.getCorners(&pts[0]);
		
		// The connections between the points
		// The corners of the box goes like this
		// In positive y, (lid of the box) 	  0-1-5-4 (-1)
		// In negative y, (bottom of the box) 2-3-7-6 (-3)
		// The '0' is +x,+y,+z while the '2' is +x,-y,+z 
		Eigen::Vector3d p0 = pts.segment(0,3);	// +x,+y,+z 
		Eigen::Vector3d p1 = pts.segment(3,3);	// +x,+y -z
		Eigen::Vector3d p2 = pts.segment(6,3);	// +x,-y,+z
		Eigen::Vector3d p3 = pts.segment(9,3);	// +x,-y,-z
		Eigen::Vector3d p4 = pts.segment(12,3);	// -x,+y,+z
		Eigen::Vector3d p5 = pts.segment(15,3);	// -x,+y,-z
		Eigen::Vector3d p6 = pts.segment(18,3);	// -x,-y,+z
		Eigen::Vector3d p7 = pts.segment(21,3);	// -x,-y,-z


		double alpha = 1/_grid_size;
		// # boxes in width
		double width = b.width() * alpha;
		// # boxes in height
		double height = b.height() * alpha;;
		// # boxes in depth
		double depth = b.depth() * alpha;;


		double dw=1;
		double dh=1;
		double dd=1;
		double lax = 1;
		if(width >= 1)
		{
			dw = 1/(lax*width);
		}else if( (width < 1) && (width > 0) ){
			dw = (depth / lax);
		}
		
		if(depth >= 1)
		{
			dd = 1/(lax*depth);
		}else if( (depth < 1) && (depth > 0) ){
			dd  = (depth / lax);
		}

		if(height >= 1 )
		{
			dh = 1/(lax*height);
		}else if( (height < 1) && (height > 0) ){
			dh = (height / lax);
		}
	

		double w = 0;
		double h = 0;
		double d = 0;
		while(w <= 1.0)
		{
			Eigen::Vector3d w_u_f = p4 + w * (p0 - p4);
			Eigen::Vector3d w_l_f = p6 + w * (p2 - p6);
			Eigen::Vector3d w_u_b = p5 + w * (p1 - p5);
			Eigen::Vector3d w_l_b = p7 + w * (p3 - p7);

			while(h <= 1.0)
			{
				Eigen::Vector3d h_f = w_l_f + h * (w_u_f - w_l_f);
				Eigen::Vector3d h_b = w_l_b + h * (w_u_b - w_l_b);

				while(d <= 1.0)
				{
					Eigen::Vector3d point = h_b + d * (h_f - h_b);
					int key = getKey(point(0), point(1), point(2));
					addBox( key, b );
					double size_div = 1/_grid_size;
					_grid[key].x[0] = floor(point(0) * size_div);
					_grid[key].x[1] = floor(point(1) * size_div);
					_grid[key].x[2] = floor(point(2) * size_div);
			//		std::cout << "KEY: " << getKey(point(0), point(1), point(2))  << "    Point   " <<  point.transpose() << std::endl;
					d += dd;
				}
				h += dh;
				d = 0;
			}
			w += dw;
			h = 0;
		}

	}while(_rList->next());
	

	// Prints the bottom corners of the gridboxes so that it can be opened in matlab
//	 PrintBoxes();
}

int CollisionEngine::getKey(double x, double y, double z)
{
	double size_div = 1/_grid_size;
	int xp = floor(x*size_div);
	int yp = floor(y*size_div);
	int zp = floor(z*size_div);
	//std::cout << "Coordinate: "<<x << ", " << y << ", " << z << std::endl;
	//std::cout << "GridPoint: "<< xp << ", " << yp << ", " << zp << std::endl;
	//std::cout << "Key: "<< xp + yp * _max_size + zp * _max_size * _max_size << std::endl;
	//std::cout << xp <<"("<< x << "), "<< yp<<"("<< y<< "), " << zp<<"("<< z<< ") " << std::endl;
	return xp + yp * _max_size + zp * _max_size * _max_size;
}

void CollisionEngine::addBox(int key, BoundingBox b)
{
	_grid[key].bb_vec.push_back(b);
}

// Get a list of indices for intersecting particles
// n is the nr of live particles and x is an array of particle positions
// N is the number of intersecting particles and ret is an array of
// indices of what particles in x are intersecting.
// Ret has to be as large as n, but is only filled with N values
void CollisionEngine::get(int n, const double* x, int& N,  int* ret)
{
	// go throgh every x,y and z index of the particles and 
	// scale them to become a key of the _grid.
	int p = 0;
	// For each living particle:
	for(int i = 0; i<n; i++)
	{
		// get its grid key
		int key = getKey(x[3*i+0], x[3*i+1], x[3*i+2]); 
		// See if the key exists as a gridbox
		if(_grid.find(key )!= _grid.end())
		{
			// Get a vector of all boundingboxes that exists in that gridbox
			auto& c = _grid.at(key).bb_vec;
			for( auto it = c.begin(); it != c.end(); it++)
			{
				// See if the particle exists is within the boundingbox
				if(it->contain(x[3*i+0], x[3*i+1], x[3*i+2]))
				{
					ret[p] = i;
					p++;
				}
			}
		}
	}
	N = p-1;
}


void CollisionEngine::PrintBoxes()
{
	printToFile("../matlab/gridBoxes.m", NULL);
	for(auto it = _grid.begin(); it != _grid.end(); it++)
	{
		double x[3] = {it->second.x[0], it->second.x[1], it->second.x[2]};
		printToFile("../matlab/gridBoxes.m",x);
	}
	
	printToFile("../matlab/boundingBoxes.m", NULL);
	_rList->first();
	do{
		BoundingBox bb = _rList->get().getBoundingBox();

		double p[24];
		bb.getCorners(p);	
		
		for(int i = 0; i<8; i++)
		{
			if(!bb.zombie())
			{
				double x[3] = {p[3*i], p[3*i+1], p[3*i+2]};
				printToFile("../matlab/boundingBoxes.m", x);
			}
		}

	}while(_rList->next());
}
