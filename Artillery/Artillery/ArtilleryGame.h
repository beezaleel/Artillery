#pragma once

#include <gdp/Engine.h>
#include <string>
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
	void DetectHit();

	GameObject* m_PlayerTank;
	GameObject* m_EnemyTank;
	Particle* m_Bullet;
};