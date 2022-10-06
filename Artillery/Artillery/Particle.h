#pragma once
#include <gdp/Engine.h>
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
	gdp::GameObject* gameObject;

	float damping;
	float mass;

	void Integrate(float dt);
	void PrintInfo();
};
