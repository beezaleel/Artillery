#pragma once

#include <gdp/Engine.h>
#include <string>
#include <vector>
#include "Particle.h"

using namespace gdp; 

class ArtilleryGame {
public:
	ArtilleryGame();
	~ArtilleryGame();

	void Initialize();
	void Destroy();

	void StartNewGame();
	void GameUpdate();
	void DisplayTextToUser(const std::string& text);

private:
	GameObject* CreateGameObjectByType(const std::string& type);
	glm::vec3 InitializePlayersPosition(const std::string type);
	int GenerateRandomNumber(int minLimit, int maxLimit);
	void UpdateCoordinate(float x, float y, float z);
	void Shoot();
	void Collision();
	void ChangeProjectileType(unsigned int bulletRounds, 
		float mass, Vector3 velocity, float damping, 
		Vector3 acceleration);
	void Clear();

	GameObject* m_PlayerTank;
	GameObject* m_EnemyTank;
	std::vector<Particle*> m_Bullets;
};