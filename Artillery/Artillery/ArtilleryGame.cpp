#include "ArtilleryGame.h"
#include <iostream>			// cout
#include "AssetInfo.h"
#include <cstdlib>
#include <ctime>
#include <math.h>

#define DEBUG_LOG_ENABLED
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

#define BOUNDARY -20

static bool seeded = false;
glm::vec3 aimCoordinate;

// Ids preloaded in the main.cpp to be referenced when
// creating specific game objects for your game.
// - EnemyTank, PlayerTank, and Bullet
unsigned int TankModelId;
unsigned int ProjectileModelId;
unsigned int PlayerMaterialId;
unsigned int EnemyMaterialId;
unsigned int BulletMaterialId;

// TODO:
// #include "YourPhysicsClass.h"

/// <summary>
/// Default constructor
/// Set all class variables to the desired default state
/// </summary>
ArtilleryGame::ArtilleryGame()
	: m_PlayerTank(nullptr)
	, m_EnemyTank(nullptr)
	, m_Bullet(nullptr) 
{
	DEBUG_PRINT("ArtilleryGame::ArtilleryGame\n");
	// DO NOTHING!!!!!!!!
}

/// <summary>
/// Default destructor
/// </summary>
ArtilleryGame::~ArtilleryGame()
{
	DEBUG_PRINT("ArtilleryGame::~ArtilleryGame\n");
	// DO NOTHING!!!!!!!!
}
/// <summary>
/// Create everything needed for your game here
/// such as memory allocation calls
/// </summary>
void ArtilleryGame::Initialize()
{
	DEBUG_PRINT("ArtilleryGame::Initialize\n");
	// TODO:
	// - Create a Player Tank GameObject
	// - Create an Enemy Tank GameObject
	// - Create projectile(s)
	m_PlayerTank = CreateGameObjectByType("Player");
	m_EnemyTank = CreateGameObjectByType("Enemy");
	m_Bullet = CreateGameObjectByType("Bullet");
	
	glm::vec3 playerPosition = InitializePlayersPosition("Player");
	m_PlayerTank->Position = playerPosition;
	m_EnemyTank->Position = InitializePlayersPosition("Enemy");

	// Bullet position will be the same as player's position
	m_Bullet->Position = playerPosition;
}

/// <summary>
/// Handle everything that needs to be destroyed
/// such as memory de-allocation.
/// </summary>
void ArtilleryGame::Destroy()
{
	DEBUG_PRINT("ArtilleryGame::Destroy\n");
	m_PlayerTank->Position = { 0, 0, 0 };
	m_PlayerTank->Renderer.MaterialId = 0;
	m_PlayerTank->Renderer.MeshId = 0;
	m_PlayerTank->Renderer.ShaderId = 0;
	m_PlayerTank->Scale = { 0, 0, 0 };

	m_EnemyTank->Position = { 0, 0, 0 };
	m_EnemyTank->Renderer.MaterialId = 0;
	m_EnemyTank->Renderer.MeshId = 0;
	m_EnemyTank->Renderer.ShaderId = 0;
	m_EnemyTank->Scale = { 0, 0, 0 };

	m_Bullet->Position = { 0, 0, 0 };
	m_Bullet->Renderer.MaterialId = 0;
	m_Bullet->Renderer.MeshId = 0;
	m_Bullet->Renderer.ShaderId = 0;
	m_Bullet->Scale = { 0, 0, 0 };
}

/// <summary>
/// StartNewGame
/// 1. Randomly place the enemy tank at a random valid location 
///    on the map
/// 2. Randomly place the player tank at a random valid location
///    on the map
/// 3. Reset the bullet(s)
/// </summary>
void ArtilleryGame::StartNewGame()
{
	DEBUG_PRINT("ArtilleryGame::StartNewGame\n");
	seeded = false;
	aimCoordinate = glm::vec3(0, 0, 0);

	Initialize();
}

/// <summary>
/// Update all bullets here
/// - Call your Physics class to update all projectiles
/// Check for bullet collision (position.y <= 0, hit ground)
/// Check for User input:
/// >>> GDP_IsKeyPressed, GDP_IsKeyHeldDown <-- Case Sensitive
/// >>> example: if (GDP_IsKeyPressed('n') || GDP_IsKeyPressed('N'))
/// - Input for changing the bullet direction	(wasd, etc)
/// - Input to change the projectile type		(1,2,3,4,5)
/// - Input to fire a projectile				(space)
/// - Input to start a new game					(n)
/// </summary>
void ArtilleryGame::GameUpdate()
{
	// DEBUG_PRINT("ArtilleryGame::GameUpdate\n");
	if (GDP_IsKeyPressed('w') || GDP_IsKeyPressed('w')) {
		DEBUG_PRINT("Key w is pressed.\n");
		UpdateCoordinate(0, 1, 0);
	}
	if (GDP_IsKeyPressed('a') || GDP_IsKeyPressed('A')) {
		DEBUG_PRINT("Key A is pressed.\n");
		UpdateCoordinate(1, 0, 0);
	}
	if (GDP_IsKeyPressed('s') || GDP_IsKeyPressed('S')) {
		DEBUG_PRINT("Key S is pressed.\n");
		UpdateCoordinate(0, -1, 0);
	}
	if (GDP_IsKeyPressed('d') || GDP_IsKeyPressed('D')) {
		DEBUG_PRINT("Key D is pressed.\n");
		UpdateCoordinate(-1, 0, 0);
	}
	if (GDP_IsKeyPressed('1')) {
		DEBUG_PRINT("Key 1 is pressed.\n");
	}
	if (GDP_IsKeyPressed('2')) {
		DEBUG_PRINT("Key 2 is pressed.\n");
	}
	if (GDP_IsKeyPressed('3')) {
		DEBUG_PRINT("Key 3 is pressed.\n");
	}
	if (GDP_IsKeyPressed('4')) {
		DEBUG_PRINT("Key 4 is pressed.\n");
	}
	if (GDP_IsKeyPressed('5')) {
		DEBUG_PRINT("Key 5 is pressed.\n");
	}
	if (GDP_IsKeyPressed(' ')) {
		DEBUG_PRINT("Key spacebar is pressed.\n");
		DEBUG_PRINT("Aimed coordinate is - (%.2f, %.2f, %.2f)\n", aimCoordinate.x, aimCoordinate.y, aimCoordinate.z);
	}
	if (GDP_IsKeyPressed('n') || GDP_IsKeyPressed('N')) {
		DEBUG_PRINT("Key N is pressed.\n");
		Destroy();
		StartNewGame();
	}
}

/// <summary>
/// Print provided text to the console for the user
/// </summary>
/// <param name="text"></param>
void ArtilleryGame::DisplayTextToUser(const std::string& text)
{
	std::cout << text << "\n";
}

GameObject* ArtilleryGame::CreateGameObjectByType(const std::string& type)
{
	DEBUG_PRINT("ArtilleryGame::CreateGameObjectByType(%s)\n", type.c_str());

	if (type.compare("Player") == 0) {
		DEBUG_PRINT("Create player!\n");
		GameObject* go = GDP_CreateGameObject();
		go->Renderer.ShaderId = 1;
		go->Renderer.MeshId = TankModelId;
		go->Renderer.MaterialId = PlayerMaterialId;
		return go;
	}
	if (type.compare("Enemy") == 0) {
		DEBUG_PRINT("Create Enemy!\n");
		GameObject* go = GDP_CreateGameObject();
		go->Renderer.ShaderId = 1;
		go->Renderer.MeshId = TankModelId;
		go->Renderer.MaterialId = EnemyMaterialId;
		return go;
	}
	if (type.compare("Bullet") == 0) {
		DEBUG_PRINT("Create Bullet!\n");
		GameObject* go = GDP_CreateGameObject();
		go->Renderer.ShaderId = 1;
		go->Renderer.MeshId = ProjectileModelId;
		go->Renderer.MaterialId = BulletMaterialId;
		go->Position = glm::vec3(0, 0, 0);
		return go;
	}

	// Invalid game object type, return nullptr
	return nullptr;
}

/// <summary>
/// Initialize start position for Player and Enemy by randomly generating x and z.
/// Set to (0, 0, 0) if type is unknown.
/// </summary>
/// <param name="type">The player type (Player | Enemy)</param>
/// <returns>Vec3 position</returns>
glm::vec3 ArtilleryGame::InitializePlayersPosition(const std::string type) {
	glm::vec3 position;
	int xPosition, zPosition;

	
	if (type == "Player") {
		xPosition = GenerateRandomNumber(BOUNDARY, abs(BOUNDARY));
		zPosition = GenerateRandomNumber(BOUNDARY, 0);
		position = glm::vec3(xPosition, 0, zPosition);
		DEBUG_PRINT("Generated vec3(%d, 0, %d) as initial position for %s\n", xPosition, zPosition, type.c_str());
	}
	else if (type == "Enemy") {
		xPosition = GenerateRandomNumber(BOUNDARY, abs(BOUNDARY));
		zPosition = GenerateRandomNumber(0, abs(BOUNDARY));
		position = glm::vec3(xPosition, 0, zPosition);
		DEBUG_PRINT("Generated vec3(%d, 0, %d) as initial position for %s\n", xPosition, zPosition, type.c_str());
	}
	else {
		position = glm::vec3(0, 0, 0);
	}

	return position;
}

/// <summary>
/// Generates a random number between 0 and maxLimit.
/// </summary>
/// <param name="minLimit">The minimum limit</param>
/// <param name="maxLimit">The maximum limit</param>
/// <returns>The generated integer</returns>
int ArtilleryGame::GenerateRandomNumber(int minLimit, int maxLimit) {
	if (!seeded) {
		srand(time(0));
		seeded = true;
	}

	return rand() % (maxLimit - minLimit) + minLimit;
}

void ArtilleryGame::UpdateCoordinate(float x, float y, float z) {
	aimCoordinate = glm::normalize(aimCoordinate + glm::vec3(x, y, z) * 0.01f);
	m_Bullet->Position = aimCoordinate;

	DEBUG_PRINT("Aimed coordinate is - (%.2f, %.2f, %.2f)\n", aimCoordinate.x, aimCoordinate.y, aimCoordinate.z);
}
