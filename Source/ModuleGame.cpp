#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
//#include "EntityManager.h"

enum PhysicCategory {

	//tangible objects
	DEFAULT = 1 << 0,
	CARS = 1 << 1,
	WALLS = 1 << 2,
	SENSORS = 1 << 3,
	DESTRUCTIBLE = 1 << 4,

};

enum PhysicGroup {
	ZONE_DETECTORS = 1
};

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleGame::~ModuleGame()
{

}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	entityManager = new EntityManager();

	//CREATION ENTITIES
	//Player
	//player = new Player(this, Vector2D(400.0f, 400.0f), EntityType::PLAYER, CARS, CARS | WALLS | DESTRUCTIBLE);
	//entityManager->AddEntity(player);
	//Ai
	
	
	//load level
	LoadLevel(1);
	CreatePlayers();
	PositionPlayersOnGrid();


	//canmera inicialization
	camera = new GameCamera(SCREEN_WIDTH, SCREEN_HEIGHT);
	camera->SetSmoothSpeed(.15f);
	camera->CenterOn(player->GetCenter());

	//call start entity manager -> call start of all entities
	entityManager->Start();

	//playerTesting = new Player(this, Vector2D(1000.0f, 1000.0f), EntityType::PLAYER, CARS, CARS);
	//entityManager->AddEntity(playerTesting);
	return ret;
}

update_status ModuleGame::PreUpdate()
{
	//BeginMode2D(camera->GetRaylibCamera());

	App->physics->SetCameraDebug(camera->GetRaylibCamera());
	return UPDATE_CONTINUE;
}


// Update: draw background
update_status ModuleGame::Update()
{
	float dt = GetFrameTime();

	if (entityManager) { entityManager->Update(dt); }
	else { std::cout<<"Entity manager update error Module Game\n"; }

	//currentMap->Update();


	if (camera && player)
	{
		camera->FollowPlayer(player);
		camera->Update(dt);
	}
	return UPDATE_CONTINUE;
}

//render in post-update
update_status ModuleGame::PostUpdate()
{
	//--------------RENDER-----------------
	//Raylib camera behaviour (start camera mode)
	BeginMode2D(camera->GetRaylibCamera());
	//render map background (floor)
	if (currentMap) { currentMap->RenderBackground(); }
	//render entities
	entityManager->Render();

	//render top elements
	if (currentMap) { currentMap->RenderTop(); }

	//draw debug physicBodies
	App->physics->DrawDebug();
	EndMode2D();
	//--------------------------------------
	//---------UI debug render--------------


	//--------------------------------------
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModuleGame::CleanUp()
{
	if (currentMap)
	{
		currentMap->CleanUp();
		delete currentMap;
	}

	if (player) {
		delete player;
		player = nullptr;
	}

	if (camera) {
		delete camera;
		camera = nullptr;
	}

	LOG("Unloading Intro scene");

	return true;
}

void ModuleGame::OnCollision(PhysBody* physA, PhysBody* physB) {

	//LOG("ModuleGame::OnCollision called");

	//if (physA && physA->entity) {
	//	Entity* entityA = physA->entity;
	//	LOG("Entity A type: %d", (int)entityA->GetType());

	//	// Puedes hacer casting si necesitas
	//	if (entityA->GetType() == EntityType::PLAYER) {
	//		Player* player = static_cast<Player*>(entityA);
	//		// Hacer algo con el player...
	//	}
	//}

	//if (physB && physB->entity) {
	//	Entity* entityB = physB->entity;
	//	LOG("Entity B type: %d", (int)entityB->GetType());
	//}


	// Obtener las entidades desde los PhysBody
	//Entity* entityA = reinterpret_cast<Entity*>(physA->body->GetUserData().pointer);
	//Entity* entityB = reinterpret_cast<Entity*>(physB->body->GetUserData().pointer);

	//if (!entityA || !entityB) {
	//	LOG("WARNING: Collision with null entity");
	//	return;
	//}

	//LOG("COLLISION: %d vs %d", (int)entityA->GetType(), (int)entityB->GetType());

	// Manejar colisiones según el tipo
	//switch (entityA->GetType()) {
	//case EntityType::PLAYER:
	//	HandlePlayerCollision(entityA, entityB);
	//	break;

	//case EntityType::TURBO_ON_ROAD:
	//	HandleTurboCollision(entityA, entityB);
	//	break;

	//case EntityType::ROCK:
	//	HandleRockCollision(entityA, entityB);
	//	break;

	//default:
	//	break;
	//}
}

void ModuleGame::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	//Entity* entityA = reinterpret_cast<Entity*>(physA->body->GetUserData().pointer);
	//Entity* entityB = reinterpret_cast<Entity*>(physB->body->GetUserData().pointer);

	//if (!entityA || !entityB) return;

	//LOG("COLLISION END: %d vs %d", (int)entityA->GetType(), (int)entityB->GetType());
}

void ModuleGame::LoadLevel(int levelNumber)
{
	// Clean other map if existed
	if (currentMap != nullptr) {
		currentMap->CleanUp();
		delete currentMap;
		currentMap = nullptr;
	}

	// CreateLevel
	switch (levelNumber) {
	case 1:
		currentMap = new Level1(App->physics, this, entityManager);
		break;
		// case 2: currentMap = new Level2(); break;
		// case 3: currentMap = new Level3(); break;
	default:
		LOG("ERROR: Invalid level number %d", levelNumber);
		return;
	}

	// Load level
	if (currentMap) {
		currentMap->Load();
		LOG("Level %d loaded", levelNumber);
	}
}

void ModuleGame::CreatePlayers()
{
	// Clean other cars
	racers.clear();

	// Crear los 4 coches (por ahora en posición temporal)
	// Posición temporal porque luego se setearán en PositionPlayersOnGrid()
	for (int i = 0; i < 4; i++) {
		Player* racer = new Player(
			this,
			Vector2D(0.0f, 0.0f),  // Posición temporal
			EntityType::PLAYER,
			CARS,
			CARS | DESTRUCTIBLE | WALLS
		);

		racers.push_back(racer);
		entityManager->AddEntity(racer);

		//the first racer created is the player
		if (i == 0) {
			player = racer;
			racer->SetIsPlayer(true);
		}
		else {
			racer->SetIsPlayer(false);
		}
	}

	// El primer coche es el controlado por el jugador
	//player = racers[0];

	// Configurar la cámara para seguir al jugador
	if (camera) {
		camera->SetTarget(player->GetCenter());
	}

	LOG("Created %d racers", racers.size());
}

void ModuleGame::PositionPlayersOnGrid()
{
	const std::vector<Vector2D>& gridPositions = currentMap->GetStartingGrid();

	// Posicionar cada coche en su posición de la parrilla
	for (size_t i = 0; i < racers.size() && i < gridPositions.size(); i++) {
		Player* racer = racers[i];
		const Vector2D& gridPos = gridPositions[i];

		// Setear la posición del Player
		racer->SetPosition(gridPos);

		// Si tiene PhysBody, actualizar también su posición física
		if (racer-> GetPhysBody() && racer->GetPhysBody()->body) {
			b2Body* body = racer->GetPhysBody()->body;
			body->SetTransform(
				b2Vec2(PIXEL_TO_METERS(gridPos.getX()), PIXEL_TO_METERS(gridPos.getY())),
				0.0f  // Rotación inicial (ajusta si es necesario)
			);
			body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));  // Velocidad inicial = 0
			body->SetAngularVelocity(0.0f);
		}

		LOG("Racer %d positioned at (%.1f, %.1f)",
			i + 1, gridPos.getX(), gridPos.getY());
	}
}
