#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP
#include <random>

#include "Geometry.hpp"
#include <Eigen/Dense>
#include <Eigen/SparseCore>

class NodeVisitor;

#ifndef PARTICLE_SYSTEM_PTR
#define PARTICLE_SYSTEM_PTR
class ParticleSystem;
typedef std::shared_ptr<ParticleSystem> partsys_ptr;
#endif // PARTICLE_SYSTEM_PTR

class ParticleSystem : public Geometry
{

	struct systemdata{
		double t;
		double K;
		int n;

		friend std::ostream& operator<<(std::ostream& os, const systemdata& data){
			os << data.t <<" " << data.K << " " << data.n;
			return os;
		}
	};
	public:

		ParticleSystem(int maxNrParticles= 1000);
		virtual ~ParticleSystem();

		void update();
		void draw();
		
		void acceptVisitor(NodeVisitor& v);
		void printToFile(std::string filename);
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

		
		GLuint particleBuffer;


		// Private functions
		void remove_dead_particles();
		void add_new_particles();


		// Rendering functions
		void createQuad();
		void createParticleBuffer();
		void sendParticlesToBuffer();

		// DEBUG etc
		float _time;
		std::vector<systemdata> _ddata;

		int totDeath;
		int totBirth;

		std::default_random_engine _gauss_num_gen;
};

#endif // PARTICLE_SYSTEM_HPP
