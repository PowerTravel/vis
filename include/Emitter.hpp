#ifndef EMITTER_HPP
#define EMITTER_HPP

#include <random>

class Emitter{

	public:
		Emitter(double mean_x=0, double mean_y=0, double mean_z=0, 
				double var_x=1, double var_y=1, double var_z=1);
		Emitter(double* mean, double* var);
		virtual ~Emitter();
	
		void generate(int n, double* v);

	private:
		
		std::default_random_engine _gauss_gen;
		std::normal_distribution<double> _x;
		std::normal_distribution<double> _y;
		std::normal_distribution<double> _z;

};

#endif // EMITTER_HPP
