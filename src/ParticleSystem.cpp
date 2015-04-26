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

	_emitter_pos = vec4(0.0,0.0,0.0,1);
	_emitter_pos_spread = vec4(0.1,0.1,0.1, 0);
 	_emitter_vel = vec4(0.0,30.0,0.0,0);
	_emitter_dir_spread = vec4(1.0,5.0,1.0,0);

	_T = mat4(1.0);
	
	_init_pos = Emitter(_emitter_pos[0],_emitter_pos[1], _emitter_pos[2],
			_emitter_pos_spread[0],_emitter_pos_spread[1],_emitter_pos_spread[2]);
	_init_vel = Emitter(_emitter_vel[0],_emitter_vel[1],_emitter_vel[2],
		_emitter_dir_spread[0], _emitter_dir_spread[1], _emitter_dir_spread[2]);

	// Render stuff
	_geom = NULL;
	createQuad();
	//load("../models/sphere.obj");
	createParticleBuffer();	
	 
	
	// Energy stuffu
	//_ddata = std::vector<Energy>();
}

void ParticleSystem::createParticleBuffer()
{
	GLuint VAO = _geom->getVAO();

	glBindVertexArray(VAO);

	glGenBuffers(1, &particleBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, particleBuffer);
	glBufferData(GL_ARRAY_BUFFER, _N*3*sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glVertexAttribPointer(Geometry::STREAM, 3, GL_FLOAT, GL_FALSE, 0,(GLvoid*) NULL);
	glEnableVertexAttribArray(Geometry::STREAM);

	glBindVertexArray(0);
}

void ParticleSystem::sendParticlesToBuffer()
{
	GLuint VAO = _geom->getVAO();
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

void ParticleSystem::updatePhysics()
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
	
	//Geometry::createGeom(4, 2, vec,  norm,  face, tex);
	_geom = geometry_ptr(new Geometry());
	_geom->createGeom(4, 2, vec, norm, face,tex);
	_bb = _geom->getBoundingBox();
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
		//Geometry::Geometry(mesh);
		_geom = geometry_ptr(new Geometry(mesh));
		_bb = _geom->getBoundingBox();
	}
}


void ParticleSystem::draw()
{
	if(_geom != NULL)
	{
		int nrFaces = _geom->getNrFaces();
		GLuint VAO = _geom->getVAO();
		glDisable(GL_CULL_FACE);
		glBindVertexArray(VAO);
		glVertexAttribDivisor(Geometry::VERTEX,0);
		glVertexAttribDivisor(Geometry::STREAM,1);
		glDrawElementsInstanced(GL_TRIANGLES,3*nrFaces,GL_UNSIGNED_INT,(void*)NULL, _n);
//		glDrawElements(GL_TRIANGLES, 3*nrFaces,GL_UNSIGNED_INT,0);
		glBindVertexArray(0);
	}
}

void ParticleSystem::acceptVisitor(NodeVisitor& v)
{
	v.apply(this);
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
