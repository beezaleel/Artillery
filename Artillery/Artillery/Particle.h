#pragma once
#include "Vector3.h"

class Particle
{
public:
	Particle();
	~Particle();

	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 scale;

	float damping;
	float mass;

	void Integrate(float dt);
};
