#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include "Geometry.hpp"
#include "PhysicsEngine.hpp"
#include "Emitter.hpp"
#include <Eigen/Dense>
#include <Eigen/SparseCore>
#include <list>

class NodeVisitor;
class Geometry;

#ifndef PARTICLE_SYSTEM_PTR
#define PARTICLE_SYSTEM_PTR
class ParticleSystem;
typedef std::shared_ptr<ParticleSystem> partsys_ptr;
#endif // PARTICLE_SYSTEM_PTR

class ParticleSystem : public PhysicsInterface
{
	struct Energy{
		double t; 	// Time
		double K; 	// Kinetic Energy
		int n;		// Nr of particles

		friend std::ostream& operator<<(std::ostream& os, const Energy& data){
			os << data.t <<" " << data.K << " " << data.n;
			return os;
		}
	};
	
	public:
		ParticleSystem();
		virtual ~ParticleSystem();

		void transform(mat4 m);

		void translate(vec3 ds);
		void translate(double dx, double dy, double dz);
		void rotate(float angle, vec3 axis);
		void rotate(float angle, double x, double y, double z);


		void updateParticlePosition(double h);
		void reflect(int n, int* id);
		void acceptPhysicsVisitor(PhysicsVisitor& v);
		void draw();
		
		void acceptVisitor(NodeVisitor& v);
		void printToFile(std::string filename);

		int getNrLiveParticles();
		int getTotNrParticles();
		const double* getParticleVec();
		

		void setCollisionSet(int n, int* id, double* point);
	private:
		struct Metadata{
			double diffuse_color;
			double cameraDistance;
			double size;
			double life;
			bool operator<(Metadata& that){
				return 	this->cameraDistance > that.cameraDistance;
			};
		};

		struct CollisionData{
			int index;
			double collision_point[3];
		};

		// Update Properties
		Eigen::Vector3d _g;
		Eigen::VectorXd _f; //Acceleration due to gravity of the system
		//	Emission Properties
		vec4 _emitter_pos;
		vec4 _emitter_vel;
		vec4 _emitter_pos_spread;
		vec4 _emitter_dir_spread;
		Emitter _init_pos;
		Emitter _init_vel;

		mat4 _T;
		mat4 _R;

		int _N;					// Max # Particles
		int _n;					// Current # Particles
		double _ppf;				// # Particles to add per frame
		double _t;
		double _pps;			// # Particles to add per second
		double _h;				// The length of each time step
		double _life; 			// Lifetime
		double _mass; 		 	// Mass

		std::vector<CollisionData> _collision_set;
		

		// Particle Properties
		//Eigen::SparseMatrix<double> _M;	// Mass matrix
		Eigen::MatrixXd _M;				// Mass matrix
		Eigen::VectorXd _x, _v;			// Position and velocity
		Metadata* _mdata;				// Metadata about the part


		// Visualization
		geometry_ptr _geom;

		// Simulation functions
		void remove_dead_particles();
		void add_new_particles();
		void calculate_forces();
		bool _forces_changed;

		// Rendering functions
		bool initGeometry(const char* filePath = NULL);
		//void createParticleBuffer();
		//void sendParticlesToBuffer();

		// Collision list
		std::list<Node*> CollisionList;

		// DEBUG etc
		std::vector<Energy> _ddata;
};

#endif // PARTICLE_SYSTEM_HPP
