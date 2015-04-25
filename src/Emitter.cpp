#include "Emitter.hpp"

Emitter::Emitter(double mean_x, double mean_y, double mean_z, 
				double var_x, double var_y, double var_z)
{
	init(mean_x, mean_y, mean_z, var_x, var_y, var_z);
}

Emitter::Emitter(double* mean, double* var)
{
	init(mean[0], mean[1], mean[2], var[0], var[1],var[2]);
}

Emitter::Emitter(vec3 mean, vec3 var)
{
	init(mean[0], mean[1], mean[2], var[0], var[1],var[2]);
}

Emitter::Emitter(vec4 mean, vec4 var)
{
	init(mean[0], mean[1], mean[2], var[0], var[1],var[2]);
}

Emitter::~Emitter()
{

}

void Emitter::init(double mean_x, double mean_y, double mean_z, 
				double var_x, double var_y, double var_z)
{
	_x = std::normal_distribution<double>(mean_x,var_x);
	_y = std::normal_distribution<double>(mean_y,var_y);
	_z = std::normal_distribution<double>(mean_z,var_z);
}

void Emitter::generate(int n, double* v)
{
	for(int i=0; i<n; ++i)
	{
		v[3*i+0] = _x(_gauss_gen);
		v[3*i+1] = _y(_gauss_gen);
		v[3*i+2] = _z(_gauss_gen);
	}
}
