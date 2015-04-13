#include "ParticleSystem.hpp"
#include "NodeVisitor.hpp"
#include <iostream>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

ParticleSystem::ParticleSystem()
{

	// Simulation Constants
	_N = 2000;
	_n = 0;
	_h = 1/60.0f;			// Timesteps
	_g << 0,-10,0;			// Gravity
	_mass = 0.1;			// Particle mass
	_ppf = 2;				// New particles per frame
	_life = 10;				// lifetime [s]
	_forces_changed = true;


	_mdata = new Metadata[_N];
	//_M = Eigen::SparseMatrix(3*_N, 3*_N);	
	//_M = Eigen::MatrixXd(3*_N, 3*_N);	
	_M = Eigen::MatrixXd(3, 3);	
	_x = Eigen::VectorXd(3*_N);	
	_v = Eigen::VectorXd(3*_N);

	_init_pos = Emitter(0.0,  0.0, 0.0, 0.1f, 0.1f, 0.1f);
	_init_vel = Emitter(0.0, 30.0, 0.0, 1.0f, 5.0f, 1.0f);

	// Render stuff
	createQuad();
	//load("../models/sphere.obj");
	createParticleBuffer();	
	 
	
	// Energy stuffu
	_ddata = std::vector<Energy>();
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
	Eigen::VectorXf x = _x.segment(0,3*_n).cast<float>();
	glBufferSubData(GL_ARRAY_BUFFER,0, _n*3*sizeof(GLfloat), &x(0));

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
		}
	
		_n += n;
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


void ParticleSystem::update()
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
//	std::cout << _n << "/" << _N <<std::endl;


	// Calculate energy
//	double k = 0;
//	double k = _v.segment(0,3*_n).transpose() * _M.block(0,0,3*_n,3*_n) * _v.segment(0,3*_n);
//	Energy sd = {_time, k, _n};
//	_ddata.push_back( sd );

	sendParticlesToBuffer();
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

void ParticleSystem::load(const char* filePath)
{

	// Let assimp read the file, Triangluate and do other
	// optimization stuff
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath,
							aiProcess_OptimizeMeshes |
							aiProcess_Triangulate | 
							aiProcess_JoinIdenticalVertices|
							aiProcess_GenSmoothNormals);
	if(!scene){
		fprintf(stderr, "Failed to load model '%s' \n '%s'\n ", filePath, importer.GetErrorString());
		return;
	}

	geometry_vec geomVec = geometry_vec(scene->mNumMeshes);
	// Start to load meshes
	aiMesh* mesh = scene-> mMeshes[0];
	if( !mesh )
	{
		fprintf(stderr, "failed to load mesh. \n");
	}else{
		// If the mesh succseded to load we create a new geometry
		// from it
		Geometry::Geometry(mesh);
	}
}


void ParticleSystem::draw()
{
	glDisable(GL_CULL_FACE);
	glBindVertexArray(VAO);
	glVertexAttribDivisor(VERTEX,0);
	glVertexAttribDivisor(STREAM,1);
	glDrawElementsInstanced(GL_TRIANGLES,3*nrFaces,GL_UNSIGNED_INT,(void*)NULL, _n);
//	glDrawElements(GL_TRIANGLES, 3*nrFaces,GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}

void ParticleSystem::acceptVisitor(NodeVisitor& v)
{
	v.apply(this);
}
