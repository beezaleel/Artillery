#include "ParticleManager.h"

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
}

Particle* ParticleManager::CreateParticle(float mass, Vector3& velocity, float damping, Vector3& acceleration) {
	Particle* particle = new Particle();

	particle->mass = mass;
	particle->velocity = velocity;
	particle->damping = damping;
	particle->acceleration = acceleration;
	//m_Particles.push_back(particle);

	//return m_Particles[m_Particles.size() - 1];
	return particle;
}

void ParticleManager::Integrate(float duration) {
	for (int i = 0; i < m_Particles.size(); i++) {
		if (m_Particles[i].position.y >= 0.f)
		{
			m_Particles[i].Integrate(duration);
			m_Particles[i].PrintInfo();
		}
	}
}