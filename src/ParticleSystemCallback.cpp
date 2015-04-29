#include "ParticleSystemCallback.hpp"
ParticleSystemCallback::ParticleSystemCallback(std::shared_ptr<ParticleSystem> ps)
{
	_pSys = ps;
}
ParticleSystemCallback::~ParticleSystemCallback(){}

void ParticleSystemCallback::execute()
{
	_pSys->translate(0.1,0,0);
}
