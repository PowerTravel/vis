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
	n->updateParticlePosition();

	int liveParticles = n->getNrLiveParticles();
	int totParticles = n->getTotNrParticles();

	const double* x = n->getParticleVec();

	int i = 0;
	int* x_n = new int[liveParticles];

	_clEng->get(liveParticles, x, i, x_n);

	n->reflect(i, x_n);
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
	delete[] x_n;
}
