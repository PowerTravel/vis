#include "PhysicsEngine.hpp"
#include "RenderList.hpp"
#include "CollisionEngine.hpp"
#include "ParticleSystem.hpp"

PhysicsEngine::PhysicsEngine(RenderList* rl, CollisionEngine* ce)
{
	_rList = rl;
	_clEng = ce;
}

PhysicsEngine::~PhysicsEngine()
{

}


void PhysicsEngine::update()
{
	_clEng->update();

	_rList->first();
	do{
		RenderNode n = _rList->get();
	
		n.acceptPhysicsVisitor(*this);
	
	}while(_rList->next());
}

void PhysicsEngine::apply(ParticleSystem* n)
{

	double fps = 60.f;;
	double fps_inv = 1/fps;
	double t = 0;
	double ticks_per_frame = 5;
	double h = 1/(fps*ticks_per_frame);
	int i = 0;
	while(t <  fps_inv){
		int liveParticles = n->getNrLiveParticles();
		int totParticles = n->getTotNrParticles();

		const double* x = n->getParticleVec();

		int* x_n = new int[liveParticles];


		_clEng->get(liveParticles, x, i, x_n);

		// Change to calculate_forces or something.
		// The point is to let the forces handle the reflection

		n->setCollisionSet(i, x_n, NULL );

		n->updateParticlePosition(h);

		t += h;
	
		delete[] x_n;
	}
/*
	if(i != 0)
	{
		std::cerr << "PhysicsEngine: " << i << std::endl;
		for( int j = 0; j<i; j++)
		{
			int part = x_n[j];
			std::cerr << "PhysicsEngine: " << part << "  " << x[3*j] << "  " << x[3*j+1]<< "  " << x[3*j+2] << std::endl;
		
		}
	}
*/
}
