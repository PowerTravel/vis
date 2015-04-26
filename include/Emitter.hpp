#ifndef EMITTER_HPP
#define EMITTER_HPP

#include <random>
#include <ctime>
#include "MatLib.hpp"

class Emitter{

	public:
		Emitter(double mean_x=0, double mean_y=0, double mean_z=0, 
				double var_x=1, double var_y=1, double var_z=1);
		Emitter(vec3 mean, vec3 var);
		Emitter(vec4 mean, vec4 var);
		Emitter(double* mean, double* var);
		virtual ~Emitter();
	
		void generate(int n, double* v);

	private:

		void init(double mean_x, double mean_y, double mean_z, 
				double var_x, double var_y, double var_z);
		
		static std::default_random_engine _gauss_gen;
		std::normal_distribution<double> _x;
		std::normal_distribution<double> _y;
		std::normal_distribution<double> _z;

};

#endif // EMITTER_HPP
