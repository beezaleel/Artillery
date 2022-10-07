#include "ParticleSystem.h"

#include <random>

ParticleSystem::ParticleSystem()
	: ParticleSystem(Vector3()) {
}

ParticleSystem::ParticleSystem(const Vector3& position)
	: m_Position(position)
	, MinAgeMilliseconds(1000.f)
	, MaxAgeMilliseconds(2000.f)
	, MinVelocity(Vector3(0.f))
	, MaxVelocity(Vector3(0.f, 1.f, 0.f))
{
}

ParticleSystem::~ParticleSystem() {
}

void ParticleSystem::GenerateParticles(unsigned int count) {
	m_Particles.resize(count);
}

// Quick utility function for a random range of two floats
float RandFloat(float min, float max) {
	if (max == min)
		return 0.f;

	// breakdown example:
	// 10.723 - 5.123 = 5.600 = 5
	// 10723 - 5123 = 5600 = 5600
	// rand() % 5600 (0, 5600) = 3123
	// 5.123 + 3.123 = 8.246

	int diff = (max - min) * 1000;
	return min + (rand() % diff) / 1000.0f;
}

void ParticleSystem::CreateParticle(
	const Vector3& velocity) {

	bool particleCreated = false;
	// NOTE: You can use an index to circularly loop the vector
	// instead of starting from 0.
	// lastUsedIndex;
	int i;
	for (i = 0; i < m_Particles.size(); i++) {
		Particle& p = m_Particles[i];
		if (p.age <= 0.f) {

			// Reset the position to the default position
			p.position = m_Position;

			// Set a random velocity between the MinVelocity and MaxVelocity
			p.velocity.x = RandFloat(MinVelocity.x, MaxVelocity.x);
			p.velocity.y = RandFloat(MinVelocity.y, MaxVelocity.y);
			p.velocity.z = RandFloat(MinVelocity.z, MaxVelocity.z);


			// Set a random age length between MinAge and MaxAge 
			// ageDifference = (MaxAge - MinAge) * 1000
			// MinAge + Rand() % ageDifference;
			int ageDiff = MaxAgeMilliseconds - MinAgeMilliseconds;

			// 3217 / 1000 = 3
			// 3217 / 1000.f = 3.217

			p.age = (MinAgeMilliseconds + rand() % ageDiff) / 1000.f;

			printf("Created a particle:\n");
			printf("  Age: %.2f\n", p.age);
			printf("  Velocity: (%.2f, %.2f, %.2f)\n",
				p.velocity.x, p.velocity.y, p.velocity.z);

			// We created a particle, leave the loop.
			particleCreated = true;
			break;
		}
	}

	if (!particleCreated) {
		printf("ParticleSystem::CreateParticle Maximum number of particles already created!\n");
		return;
	}
}

void ParticleSystem::Integrate(
	float duration) {
	for (int i = 0; i < m_Particles.size(); i++) {
		Particle& p = m_Particles[i];
		if (p.age > 0.f) {
			p.Integrate(duration);
		}
	}
}

void ParticleSystem::PrintParticleInfo() {
	for (int i = 0; i < m_Particles.size(); i++) {
		Particle& p = m_Particles[i];
		if (p.age > 0.f) {
			p.PrintInfo();
		}
	}
}