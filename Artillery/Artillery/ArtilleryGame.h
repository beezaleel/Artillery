#pragma once

#include <gdp/Engine.h>
#include <string>

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

	GameObject* m_PlayerTank;
	GameObject* m_EnemyTank;
	GameObject* m_Bullet;
};