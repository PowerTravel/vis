#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include "Node.hpp"
#include <Eigen/Dense>
#include <Eigen/SparseCore>

class NodeVisitor;

class ParticleSystem : public Node
{

	public:

		ParticleSystem(int maxNrParticles= 100);
		virtual ~ParticleSystem();

		void update();
		void draw();
		
		void acceptVisitor(NodeVisitor& v);
	private:
		struct metadata{
			double cameraDistance;
			double life;
			bool operator<(metadata& that){
				return 	this->cameraDistance > that.cameraDistance;
			};
		};

		// Update Properties
		double _h;				// The length of each time step
		Eigen::Vector3d _g;
		Eigen::VectorXd _f; //Acceleration due to gravity of the system
		//	Emission Properties
		Eigen::Vector3d _pos;  // Initial Position;
		Eigen::Vector3d _var_p;   // Variance 
		Eigen::Vector3d _vel;  // Initial Velocity;
		Eigen::Vector3d _var_v;   // Variance 
		int _N;					// Max # Particles
		int _n;					// Current # Particles
		int _ppf;				// # Particles to add per frame
		double _life, _mass;  	// Initial life and mass

		// Particle Properties
		//Eigen::SparseMatrix<double> _M;	// Mass matrix
		Eigen::MatrixXd _M;	// Mass matrix
		Eigen::VectorXd _x, _v;			// Position and velocity
		metadata* _mdata;				// Metadata about the part

		// Private functions
		void remove_dead_particles();
		void add_new_particles();

		// DEBUG etc
		std::vector<double> E;

};

#endif // PARTICLE_SYSTEM_HPP