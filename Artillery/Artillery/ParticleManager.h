#pragma once

#include "Particle.h"
#include <vector>

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	Particle* CreateParticle(float mass, Vector3& velocity, float damping, Vector3& acceleration);
	void Integrate(float duration);

private:
	std::vector<Particle> m_Particles;
};
