#include "ParticleSystem.hpp"
#include "NodeVisitor.hpp"
#include <iostream>
#include <fstream>

ParticleSystem::ParticleSystem(int maxNrParticles)
{
	_N = maxNrParticles;
	_n = 0;

	// (for now) Hardcoded Constants
	_h = 1/60.0f;			// Timesteps
	_g << 0,9.82,0;			// Gravity
	_pos << 0,0,0;			// emission postition
	_var_p  << 0.5,0.5,0.5;	// emission position variance	
	_vel << 0,0.2,0;		// emission velocity
	_var_v << 0.5,0.5,0.5;	// emission velocity variance
	_mass = 1;				// Particle mass
	_ppf = 1;				// New particles per frame
	_life = 1;				// lifetime [s]

	_f = Eigen::VectorXd(3*_N);
	for(int i = 0; i<_N; ++i)
	{
		_f.segment(3*i,3) = _g*_mass;
	}


	//_M = Eigen::SparseMatrix(3*_N, 3*_N);	
	_M = Eigen::MatrixXd(3*_N, 3*_N);	
	_x = Eigen::VectorXd(3*_N);	
	_v = Eigen::VectorXd(3*_N);

	_mdata = new metadata[maxNrParticles];
	std::default_random_engine _gauss_num_gen;

		
	 _ddata = std::vector<systemdata>();
	
	// Debugconstants
	 _time = 0;
	 totDeath = 0;
	 totBirth = 0;

	 // Render related stuff	
	createQuad();
	createParticleBuffer();	
}
void ParticleSystem::createParticleBuffer()
{
	glBindVertexArray(VAO);

	glGenBuffers(1, &particleBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
	glBufferData(GL_ARRAY_BUFFER, _N*3*sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glVertexAttribPointer(STREAM, 3, GL_FLOAT, GL_FALSE, 0,(GLvoid*) NULL);
	glEnableVertexAttribArray(STREAM);

	glBindVertexArray(0);
}

void ParticleSystem::sendParticlesToBuffer()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
	glBufferData(GL_ARRAY_BUFFER, _N*3*sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0, _n*3*sizeof(GLfloat), &_x(0));
	
	glBindVertexArray(0);
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
		totDeath ++;
	}

	for(int i=0; i<_n; ++i)
	{
		// If the life of a particle has expired
		if(_mdata[i].life <= 0)
		{
			// Swap the dead particle with the last particle
			metadata swap = _mdata[i];
			_mdata[i] = _mdata[_n-1];
			_mdata[_n-1] = swap;
			
			// Do the same with position, velocity and mass
			// but we don't have to swap this time
			_x.segment(3*i, 3) = _x.segment(3*(_n-1), 3);
			_v.segment(3*i, 3) = _v.segment(3*(_n-1), 3);
			_M.block(3*i, 3*i, 3, 3) = _M.block(3*(_n-1), 3*(_n-1), 3, 3);

			_n--;
			totDeath ++;
		}
		
		// Decrease the life of the particle
		_mdata[i].life -= _h;
	}
}

void ParticleSystem::add_new_particles()
{
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
	std::normal_distribution<double> dist_pos_x = 
					std::normal_distribution<double>(_pos(0),_var_p(0));
	std::normal_distribution<double> dist_pos_y =
					std::normal_distribution<double>(_pos(1),_var_p(1));
	std::normal_distribution<double> dist_pos_z =
					std::normal_distribution<double>(_pos(2),_var_p(2));
	
	std::normal_distribution<double> dist_vel_x = 
					std::normal_distribution<double>(_vel(0),_var_v(0));
	std::normal_distribution<double> dist_vel_y = 
					std::normal_distribution<double>(_vel(1),_var_v(1));
	std::normal_distribution<double> dist_vel_z = 
					std::normal_distribution<double>(_vel(2),_var_v(2));
	//std::cout << _pos.transpose() << " : " << _var_p.transpose() <<" : " << _vel.transpose()  <<" : " <<_var_v.transpose() <<std::endl;
	//std::cout<< (rand()/(double)(RAND_MAX)) << std::endl;
	for(int i = 0; i<n; ++i)
	{
		 // Generate a gaussian distribution
		_x(_n+3*i+0) = dist_pos_x(_gauss_num_gen);
		_x(_n+3*i+1) = dist_pos_y(_gauss_num_gen);
		_x(_n+3*i+2) = dist_pos_z(_gauss_num_gen);
    	_v(_n+3*i+0) = dist_vel_x(_gauss_num_gen);
    	_v(_n+3*i+1) = dist_vel_y(_gauss_num_gen);
    	_v(_n+3*i+2) = dist_vel_z(_gauss_num_gen);
		_M.block(_n+3*i,_n+3*i,3,3) =_mass * Eigen::MatrixXd::Identity(3,3);
		
		// Set metadata
		_mdata[_n+i].life = _life;
	}

	totBirth += n;
	_n += n;
}

void ParticleSystem::update()
{
	// Remove dead particles
	remove_dead_particles();
	
	// Emit new particles
	add_new_particles();

	// Update living particles
	_v.segment(0,_n) = _v.segment(0,_n) + _h*_f.segment(0,_n);
	_x.segment(0,_n) = _x.segment(0,_n) + _h*_v.segment(0,_n);
	//std::cout << _v.segment(0,3).transpose() <<std::endl;


	// Calculate energy
	double k = 0;
//	double k = _v.segment(0,3*_n).transpose() * _M.block(0,0,3*_n,3*_n) * _v.segment(0,3*_n);
//	systemdata sd = {_time, k, _n};
//	_ddata.push_back( sd );

	_time += _h;

	sendParticlesToBuffer();
//	printToFile("bajsloek.m");
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

void ParticleSystem::createQuad()
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
	
	Geometry::createGeom(4, 2, vec,  norm,  face, tex);
}

void ParticleSystem::draw()
{
	glPointSize(4.0);
	glDisable(GL_CULL_FACE);
	glBindVertexArray(VAO);
	glVertexAttribDivisor(VERTEX,0);
	glVertexAttribDivisor(STREAM,1);
	glDrawElementsInstanced(GL_TRIANGLES,3*nrFaces,GL_UNSIGNED_INT,(void*)NULL, _n);
	//glDrawElements(GL_TRIANGLES, 3*nrFaces,GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
	glPointSize(1.0);
}

void ParticleSystem::acceptVisitor(NodeVisitor& v)
{
	v.apply(this);
}
