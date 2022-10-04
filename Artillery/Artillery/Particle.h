#pragma once

#include <gdp/MeshRenderer.h>
#include "Vector3.h"

using namespace gdp;

class Particle
{
public:
	Particle();
	~Particle();

	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 scale;
	MeshRenderer renderer;
};
