#include "ArtilleryGame.h"
#include "AssetInfo.h"
#include "ParticleManager.h"
#include <cstdlib>
#include <ctime>
#include <iostream>			// cout
#include <math.h>
#include <vector>

#define DEBUG_LOG_ENABLED
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

#define BOUNDARY -20

bool isShooting;
bool isGameOver;
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
	, m_Bullets()
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
	
	glm::vec3 playerPosition = InitializePlayersPosition("Player");
	m_PlayerTank->Position = playerPosition;
	m_EnemyTank->Position = InitializePlayersPosition("Enemy");

	// Creating and pusing the first bullet.
	Particle* bullet = new Particle();
	bullet->gameObject = CreateGameObjectByType("Bullet");
	bullet->velocity = Vector3(0.0f, 0.0f, 5.0f);
	bullet->acceleration = Vector3(0.0f, -0.981f, 0.0f);
	bullet->mass = 1.0f;
	bullet->damping = 1.0f;

	m_Bullets.push_back(bullet);
	DisplayTextToUser("Using default projectile...");
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

	// Ensure memory is deallocated for the object created with new.
	Clear();
	m_Bullets.resize(0);
	m_Bullets.shrink_to_fit();
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
	isShooting = false;
	isGameOver = false;
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
	// Press any key to change target direction.
	if (GDP_IsKeyPressed('s') || GDP_IsKeyPressed('S')) {
		UpdateCoordinate(-2, 0, 0);
	}
	if (GDP_IsKeyPressed('e') || GDP_IsKeyPressed('E')) {
		UpdateCoordinate(0, 2, 0);
	}
	if (GDP_IsKeyPressed('d') || GDP_IsKeyPressed('D')) {
		UpdateCoordinate(0, -2, 0);
	}
	if (GDP_IsKeyPressed('q') || GDP_IsKeyPressed('Q')) {
		UpdateCoordinate(0, 0, 2);
	}
	if (GDP_IsKeyPressed('a') || GDP_IsKeyPressed('A')) {
		UpdateCoordinate(0, 0, -2);
	}
	if (GDP_IsKeyPressed('w') || GDP_IsKeyPressed('W')) {
		UpdateCoordinate(2, 0, 0);
	}
	
	if (GDP_IsKeyPressed('1')) {
		Vector3 velocity = Vector3(0.0f, 0.0f, 35.0f);
		Vector3 acceleration = Vector3(0.0f, -1.235f, 0.0f);
		ChangeProjectileType(1, 2.0f, velocity, 1.0f, acceleration);
		DisplayTextToUser("Changing projectile type to PISTOL...");
	}
	if (GDP_IsKeyPressed('2')) {
		Vector3 velocity = Vector3(0.0f, 30.0f, 40.0f);
		Vector3 acceleration = Vector3(0.0f, -1.5f, 0.0f);
		ChangeProjectileType(1, 200.0f, velocity, 1.0f, acceleration);
		DisplayTextToUser("Changing projectile type to ARTILLERY...");
	}
	if (GDP_IsKeyPressed('3')) {
		Vector3 velocity = Vector3(0.0f, 0.0f, 10.0f);
		Vector3 acceleration = Vector3(0.0f, -0.6f, 0.0f);
		ChangeProjectileType(1, 1.0f, velocity, 1.0f, acceleration);
		DisplayTextToUser("Changing projectile type to FIREBALL...");
	}
	if (GDP_IsKeyPressed('4')) {
		Vector3 velocity = Vector3(0.0f, 0.0f, 100.0f);
		Vector3 acceleration = Vector3(0.0f, -0.3f, 0.0f);
		ChangeProjectileType(1, 0.1f, velocity, 1.0f, acceleration);
		DisplayTextToUser("Changing projectile type to LASER...");
	}
	if (GDP_IsKeyPressed('5')) {
		Vector3 velocity = Vector3(0.0f, 40.0f, 60.0f);
		Vector3 acceleration = Vector3(0.0f, -0.734f, 0.0f);
		ChangeProjectileType(1, 400.0f, velocity, 1.0f, acceleration);
		DisplayTextToUser("Changing projectile type to ROCKET LAUNCHER...");
	}
	if (GDP_IsKeyPressed(' ')) {
		DEBUG_PRINT("Aimed coordinate is - (%.2f, %.2f, %.2f)\n", aimCoordinate.x, aimCoordinate.y, aimCoordinate.z);
		Shoot();
	}
	if (GDP_IsKeyPressed('n') || GDP_IsKeyPressed('N')) {
		Destroy();
		StartNewGame();
	}

	if (isShooting) {
		for (int i = 0; i < m_Bullets.size(); i++) {
			m_Bullets[i]->Integrate(0.005f);

			if (m_Bullets[i]->gameObject->Position.y <= 0.0f) {
				Collision();
				isShooting = false;
			}
		}
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

/// <summary>
/// Creates a new Game object based on type name
/// </summary>
/// <param name="type">The name of type to create</param>
/// <returns></returns>
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
	
	if (type == "Enemy") {
		xPosition = GenerateRandomNumber(BOUNDARY, abs(BOUNDARY));
		zPosition = GenerateRandomNumber(BOUNDARY, 0);
		position = glm::vec3(xPosition, 0, zPosition);
		DEBUG_PRINT("Generated vec3(%d, 0, %d) as initial position for %s\n", xPosition, zPosition, type.c_str());
	}
	else if (type == "Player") {
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

/// <summary>
/// Update the target coordinate
/// </summary>
/// <param name="x">x axis</param>
/// <param name="y">y axis</param>
/// <param name="z">z axis</param>
void ArtilleryGame::UpdateCoordinate(float x, float y, float z) {
	for (int i = 0; i < m_Bullets.size(); i++) {
		aimCoordinate = glm::normalize(aimCoordinate + glm::vec3(x, y, z) * 0.01f);
		m_Bullets[i]->gameObject->Position = aimCoordinate;
	}
	DEBUG_PRINT("Aimed coordinates: (%.2f, %.2f, %.2f)\n", aimCoordinate.x, aimCoordinate.y, aimCoordinate.z);
}

/// <summary>
/// Shoot when the spacebar key is pressed. Involves updating the position and velocity of the particles
/// </summary>
void ArtilleryGame::Shoot() {
	if (isShooting)
		return;

	for (int i = 0; i < m_Bullets.size(); i++) {
		m_Bullets[i]->gameObject->Position = m_PlayerTank->Position + glm::vec3(0.0f, 1.0f, 0.0f);
		m_Bullets[i]->velocity = Vector3(aimCoordinate.x, aimCoordinate.y, aimCoordinate.z) * 6.0f;
	}
	isShooting = true;
}

/// <summary>
/// Detect if the bullet has hit target. Calculate distance between bullet and target.
/// </summary>
void ArtilleryGame::Collision() {
	for (int i = 0; i < m_Bullets.size(); i++) {
		float distance = glm::distance(m_Bullets[i]->gameObject->Position, m_EnemyTank->Position);
		if (distance < 1.5f) {
			DisplayTextToUser("Enemy eliminated!. Please press `n` to start a new game.\n");
			isGameOver = true;
			return;
		}
		else
		{
			DEBUG_PRINT("Missed target by %d meters!\n", (int)distance);
		}
	}
}

/// <summary>
/// Change projectile type from PISTOL, ARTILLERY, FIREBALL, LASER OR ROCKET LAUNCHER
/// </summary>
/// <param name="bulletRounds">Number of particles to project</param>
/// <param name="mass">The weight of the particle</param>
/// <param name="velocity">The velocity of the particle</param>
/// <param name="damping">Damping force</param>
/// <param name="acceleration">The acceleration</param>
void ArtilleryGame::ChangeProjectileType(unsigned int bulletRounds, float mass, Vector3 velocity, float damping, Vector3 acceleration) {
	Clear();
	ParticleManager particleManager;
	m_Bullets.resize(bulletRounds);
	for (int i = 0; i < m_Bullets.size(); i++) {
		m_Bullets[i] = particleManager.CreateParticle(mass, velocity, damping, acceleration);
		m_Bullets[i]->gameObject = CreateGameObjectByType("Bullet");
	}
}

/// <summary>
/// Manual cleanup of m_Bullets objects
/// </summary>
void ArtilleryGame::Clear() {
	if (m_Bullets.size() > 0) {
		for (int i = 0; i < m_Bullets.size(); i++) {
			m_Bullets[i]->acceleration = { 0, 0, 0 };
			m_Bullets[i]->gameObject->Position = { 0, 0, 0 };
			m_Bullets[i]->gameObject->Renderer.MaterialId = 0;
			m_Bullets[i]->gameObject->Renderer.MeshId = 0;
			m_Bullets[i]->gameObject->Renderer.ShaderId = 0;
			m_Bullets[i]->gameObject->Scale = { 0, 0, 0 };
			m_Bullets[i]->velocity = { 0, 0, 0 };
		}
	}
}
