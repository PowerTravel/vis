#ifndef PARTICLE_SYSTEM_CALLBACK_HPP
#define PARTICLE_SYSTEM_CALLBACK_HPP

#include "ParticleSystem.hpp"

class ParticleSystemCallback : public UpdateCallback{ 
	
	public:
		ParticleSystemCallback(std::shared_ptr<ParticleSystem> ps);
		virtual ~ParticleSystemCallback();

		void execute();

	private:
		std::shared_ptr<ParticleSystem> _pSys;
};


#endif // PARTICLE_SYSTEM_CALLBACK_HPP
