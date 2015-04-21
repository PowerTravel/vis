#include "CollisionEngine.hpp"
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
	
	// Reset Grid
	_grid = std::map<int, container>();
	std::list<RenderNode>& list = (_rl->list);
	for(auto it = list.begin(); it != list.end(); it++)
	{
		std::cout<<"COLLISIONENGINE UPDATE" << std::endl;
		// Check all the corners in the container and add the bb to them
		
		BoundingBox b = it->getBoundingBox();
		double p[24];
		b.getCorners(p);

		
		std::map<int,int> key_map = std::map<int,int>();
		int key = getKey(p[0], p[1], p[2]);
		key_map.insert(std::pair<int,int>(key,0) );
		addBox(key, b);
		for(int i=1; i<8; i++)
		{
			key = getKey(p[3*i+0], p[3*i+1], p[3*i+2]);
			
			// We add only a  bounding box to each container in _grid once
			if(key_map.find(key) == key_map.end())
			{
				key_map.insert(std::pair<int,int>(key,i) );
				addBox(key, b);
			}
		}	
		// if the key_map added more than one value, we may have some 
		// interior conatiners to fill
		if( key_map.size() > 1 )
		{
			// Fill in the interior containers,
			// The corners are filled by the points in p whose index we
			// find as values in key_map
			
			for(auto key_it = key_map.begin(); key_it!=key_map.end(); key_it++){
				std::cout << key_it->first << "  "<< key_it->second << "   " << p[3 * key_it->second + 0]<< " " << p[3 * key_it->second + 1]<< " " <<  p[3 * key_it->second + 2]  <<std::endl;
			}
		
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

}

// Get a list of indices for intersecting particles
// n is the nr of particles and x is an array of particle positions
// N is the number of intersecting particles and ret is an array of
// indices of what particles in x are intersecting.
// Ret has to be as large as n, but is only filled with N values
void CollisionEngine::get(int n, const double* x, int* N,  int* ret)
{
		// go throgh every x,y and z index of the particles and 
		// scale them to become a key of the _grid.

		// Check if the key exists in the _grid. If it does it means
		// it is occupied by a bounding box. See if the point intersects
		// the bounding box .
		// If so, add the index to the vector. and increment N.
}
