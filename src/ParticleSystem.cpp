#include "ParticleSystem.hpp"
#include "NodeVisitor.hpp"
#include <iostream>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

ParticleSystem::ParticleSystem()
{
	// parameters 
	_N = 100; 				// Max number of allowed particles
	_pps = 10;				// particles created per seocnd
	_life = 10;				// lifetime of particles in seconds
	
	_emitter_pos = vec4(0.0,3.0,0.0,1);   	// mean position
	_emitter_pos_spread = vec4(1,1,1, 0);	// spread
 	_emitter_vel = vec4(1.0,0.0,1.0,0);		// mean spead
	_emitter_dir_spread = vec4(1,1,1,0);	// spread
	// Simulation Constants
	_n = 0;
	_h = 1/60.0f;			// Timesteps
	_g << 0,-10,0;			// Gravity
	_mass = 0.1;			// Particle mass
	_ppf = 0;				// New particles per frame
	_forces_changed = true;

	_mdata = new Metadata[_N];
	//_M = Eigen::SparseMatrix(3*_N, 3*_N);	
	//_M = Eigen::MatrixXd(3*_N, 3*_N);	
	_M = Eigen::MatrixXd(3, 3);	
	_x = Eigen::VectorXd(3*_N);	
	_v = Eigen::VectorXd(3*_N);


	_T = mat4(1.0);

	_t = 0;

	_init_pos = Emitter(_emitter_pos, _emitter_pos_spread);
	_init_vel = Emitter(_emitter_vel, _emitter_dir_spread);

	_geom = NULL;
	if( !initGeometry("../models/sphere.obj"))
	{
		_geom = NULL;
		std::cerr << "ParticleSystem failed to load geometry. "<< std::endl;
	}

	// Energy stuffu
	//_ddata = std::vector<Energy>();
}


ParticleSystem::~ParticleSystem()
{
	if(_mdata != NULL)
	{
		delete [] _mdata;
	}
}

void ParticleSystem::remove_dead_particles()
{
	// Remove all tailing dead particles
	// so that we know that the last particle is living
	while(_mdata[_n-1].life<=0 && _n>0)
	{
		_n--;
	}

	for(int i=0; i<_n; ++i)
	{
		// If the life of a particle has expired
		if(_mdata[i].life <= 0)
		{
			// Swap the dead particle with the last particle
			Metadata swap = _mdata[i];
			_mdata[i] = _mdata[_n-1];
			_mdata[_n-1] = swap;
			
			// Do the same with position, velocity and mass
			// but we don't have to swap this time
			_x.segment(3*i, 3) = _x.segment(3*(_n-1), 3);
			_v.segment(3*i, 3) = _v.segment(3*(_n-1), 3);
			//_M.block(3*i, 3*i, 3, 3) = _M.block(3*(_n-1), 3*(_n-1), 3, 3);
			_n--;
		}
		
		// Decrease the life of the particle
		_mdata[i].life -= _h;
	}
}

void ParticleSystem::add_new_particles()
{
	if(_n < _N){
		_ppf =_ppf + _pps * _h;

		int n = _ppf; // The number of new particles to add
		int spaceLeft = _N - _n;
		if(n > spaceLeft)
		{
			n = spaceLeft;
		}
		if(n < 0){
			n = 0;
		}
		// Add new particles to _x, _v, _M.
		// _x and _v are added with a gaussian distribution
		_init_pos.generate(n, &_x[3*_n]);
		_init_vel.generate(n, &_v[3*_n]);
		//_M.block(3*_n,3*_n,3*n,3*n) =_mass * Eigen::MatrixXd::Identity(3*n,3*n);
	
		for(int i = 0; i<n; ++i)
		{
			// Set metadata
			_mdata[_n+i].life = _life;
			_mdata[_n+i].diffuse_color = (double)( rand() % 1000) /1000.f;
			//std::cerr << _mdata[_n+i].diffuse_color << std::endl;
		}
	
		_n += n;
		if(_ppf >=  1)
		{
			_ppf = 0;
		}
	}
}

void ParticleSystem::calculate_forces()
{
	if(_forces_changed)
	{
		_f = Eigen::VectorXd(3*_N);
		for(int i = 0; i<_N; ++i)
		{
			_f.segment(3*i,3) = _g*_mass;
		}
		_forces_changed = false;
	}
}

void ParticleSystem::acceptPhysicsVisitor(PhysicsVisitor& v)
{
	v.apply(this);

	// Calculate energy
//	double k = 0;
//	double k = _v.segment(0,3*_n).transpose() * _M.block(0,0,3*_n,3*_n) * _v.segment(0,3*_n);
//	Energy sd = {_time, k, _n};
//	_ddata.push_back( sd );
}

void ParticleSystem::updateParticlePosition()
{
	// Remove dead particles
	remove_dead_particles();
	
	// Emit new particles
	add_new_particles();

	// Calc Forces
	calculate_forces();

	// Update living particles
	_v.segment(0,3*_n) = _v.segment(0,3*_n) + (_h/_mass)*_f.segment(0,3*_n);
	_x.segment(0,3*_n) = _x.segment(0,3*_n) + _h*_v.segment(0,3*_n);
	_t += _h;
}

void ParticleSystem::printToFile(std::string filename)
{
	std::ofstream file;
	file.open(filename, std::ios::out | std::ios::trunc);
	for(int i = 0; i<_ddata.size(); ++i)
	{
		file << _ddata[i] <<  std::endl;
	}
	file.close();
}

bool ParticleSystem::initGeometry(const char* filePath)
{
	if(filePath == NULL)
	{
		float vec[] = {	-0.5,-0.5,0,
						0.5,-0.5,0,
						-0.5,0.5,0,
						0.5,0.5,0};	
	
		float norm[] = {	0,0,1,
							0,0,1,
							0,0,1,
							0,0,1};
	
		int face[] = { 	0,1,2,
				   		1,3,2};
	
		float tex[] = {	0,0,
						1,0,
						0,1,
						1,1};
		
		_geom = geometry_ptr(new Geometry(4, 2, vec, norm, face,tex));
	}else{
		_geom = geometry_ptr(new Geometry(filePath));
	}

	if(_geom != NULL  && !_geom->zombie())
	{
		_bb = _geom->getBoundingBox();
		return true;
	}else{
		return false;	
	}
}


void ParticleSystem::load(const char* filePath)
{
}


void ParticleSystem::draw()
{
	if(_geom != NULL && _n != 0)
	{
		Eigen::VectorXf x = _x.segment(0,3*_n).cast<float>();
		_geom->draw(_N,_n,&x(0));
	}
}

void ParticleSystem::acceptVisitor(NodeVisitor& v)
{
	v.apply(this);
}


void ParticleSystem::reflect(int n, int* id)
{

	for(int i = 0; i<n; i++)
	{
		//std::cout << 3*id[i]+1 << " " << _v.size() << _v(3*id[i]+1) << std::endl;
		_v(3*id[i]+1)  = -0.6 *  _v(3*id[i]+1);
		if(_v(3*id[i]+1)< 1 || _v(3*id[i]+1)>-1)
			_v(3*id[i]+1) = 2;
	}
}



void ParticleSystem::translate(vec3 ds)
{
	mat4 m = mat4(1.0);
	m = glm::translate(m,ds);
	transform(m);
}

void ParticleSystem::translate(double dx, double dy, double dz)
{
	translate(vec3(dx,dy,dz));
}

void ParticleSystem::rotate(float angle, vec3 axis)
{
}

void ParticleSystem::rotate(float angle, double x, double y, double z)
{
	rotate(angle,vec3(x,y,z));
}

void ParticleSystem::transform(mat4 m)
{
	_T = _T*m;
	vec4 pos = _T * vec4(0,0,0,1);
	_init_pos = Emitter( pos,_emitter_pos_spread );
}
		
int ParticleSystem::getNrLiveParticles()
{
	return _n;
}
const double* ParticleSystem::getParticleVec()
{
	return &_x(0);
}
int ParticleSystem::getTotNrParticles()
{
	return _N;
}
