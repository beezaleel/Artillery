#include <iostream>
#include "Particle.h"

Particle::Particle() {
	position = 0.0f;
	velocity = 0.0f;
	acceleration = 0.0f;
	damping = 1.0f;
	mass = 1.0f;
}

Particle::~Particle() {
}

void Particle::Integrate(float dt) {
	velocity.addScaledVector(acceleration, dt);
	velocity *= damping;
	position.addScaledVector(velocity, dt);
}

void Particle::PrintInfo() {
	std::cout << "-----------------\n";
	std::cout << "Velocity(" << velocity.x << ", " << velocity.y << ", " << velocity.z << ")\n";
	std::cout << "Position(" << position.x << ", " << position.y << ", " << position.z << ")\n";
}
