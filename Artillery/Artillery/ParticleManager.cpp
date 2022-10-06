#include "ParticleManager.h"

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
}

Particle& ParticleManager::CreateParticle(Vector3& position) {
	Particle particle;

	particle.position = position;
	particle.acceleration = Vector3(0.0f, -9.8f, 0.0f);
	particle.mass = 0.01f;
	m_Particles.push_back(particle);

	return m_Particles[m_Particles.size() - 1];
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