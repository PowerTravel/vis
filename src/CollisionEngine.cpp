#include "CollisionEngine.hpp"
#include "Eigen/Dense"
#include <iostream>

CollisionEngine::CollisionEngine(RenderList* rl)
{
	_rl = rl;
	_max_size = 1000;
	_grid_size = 4;
	_grid = std::map<int, container>();
}

CollisionEngine::~CollisionEngine(){}

void CollisionEngine::update()
{
	if(_rl == NULL)
	{
		return;	
	}


	_grid = std::map<int, container>();
	std::list<RenderNode>& list = (_rl->list);
	for(auto it = list.begin(); it != list.end(); it++)
	{
		BoundingBox b = it->getBoundingBox();
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

		//std::cout <<_grid_size << "  "  <<width << " " << height<< " " << depth << std::endl;

		double w = 0;
		double h = 0;
		double d = 0;
		double dw, dh, dd;
		if(width > 0)
		{
			dw = 1/width;
		}else{
			dw = 1;
		}

		if(depth > 0)
		{
			dh = 1/height;
		}else{
			dh = 1;
		}
		
		if(height > 0)
		{
			dd = 1/depth;
		}else{
			dd = 1;
		}
	
		while(w <= 1)
		{
			Eigen::Vector3d w_u_f = p4 + w * (p0 - p4);
			Eigen::Vector3d w_l_f = p6 + w * (p2 - p6);
			Eigen::Vector3d w_u_b = p5 + w * (p1 - p5);
			Eigen::Vector3d w_l_b = p7 + w * (p3 - p7);

			while(h <= 1)
			{
				Eigen::Vector3d h_f = w_l_f + h * (w_u_f - w_l_f);
				Eigen::Vector3d h_b = w_l_b + h * (w_u_b - w_l_b);

				while(d <= 1)
				{
					Eigen::Vector3d point = h_b + d * (h_f - h_b);
					addBox( getKey(point(0), point(1), point(2)) , b );
					d += dd;
				}
				h += dh;
				d = 0;
			}
			w += dw;
			h = 0;
		}
	}
}

int CollisionEngine::getKey(double x, double y, double z)
{
	double size_div = 1/_grid_size;
	int xp = floor(x*size_div);
	int yp = floor(y*size_div);
	int zp = floor(z*size_div);
	return xp + yp * _max_size + zp * _max_size * _max_size;
}

void CollisionEngine::addBox(int key, BoundingBox b)
{
	_grid[key].bb_vec.push_back(b);
}

// Get a list of indices for intersecting particles
// n is the nr of particles and x is an array of particle positions
// N is the number of intersecting particles and ret is an array of
// indices of what particles in x are intersecting.
// Ret has to be as large as n, but is only filled with N values
void CollisionEngine::get(int n, const double* x, int& N,  int* ret)
{
		// go throgh every x,y and z index of the particles and 
		// scale them to become a key of the _grid.
		N = 0;
		for(int i = 0; i<n; i++)
		{	
			int key = getKey(x[3*i+0], x[3*i+1], x[3*i+2]); 
			if(_grid.find(key ) != _grid.end())
			{
				auto& c = _grid.at(key).bb_vec;
				for( auto it = c.begin(); it != c.end(); it++)
				{
					if(it->contain(x[3*i+0], x[3*i+1], x[3*i+2]))
					{
						ret[N] = i;
						N++;
					}
				}
			}
		}
}
