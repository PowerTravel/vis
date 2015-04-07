#include "Emitter.hpp"

Emitter::Emitter(double mean_x, double mean_y, double mean_z, 
				double var_x, double var_y, double var_z)
{
	_x = std::normal_distribution<double>(mean_x,var_x);
	_y = std::normal_distribution<double>(mean_y,var_y);
	_z = std::normal_distribution<double>(mean_z,var_z);
}

Emitter::Emitter(double* mean, double* var)
{
	_x = std::normal_distribution<double>(mean[0],var[0]);
	_y = std::normal_distribution<double>(mean[1],var[1]);
	_z = std::normal_distribution<double>(mean[2],var[2]);
}

Emitter::~Emitter()
{

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
