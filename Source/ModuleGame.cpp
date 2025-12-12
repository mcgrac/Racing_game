#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
//#include "EntityManager.h"

//enum PhysicCategory {
//
//	//tangible objects
//	DEFAULT = 1 << 0,
//	CARS = 1 << 1,
//	WALLS = 1 << 2,
//	SENSORS = 1 << 3,
//	DESTRUCTIBLE = 1 << 4,
//
//};

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

	//loop music level
	currentMap->UpdateMusic();


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

	std::cout << "ModuleGame::OnCollision called" <<std::endl;

	//Check which is the player and which is the other entity
	Entity* player = nullptr;
	Entity* other = nullptr;
	//if ((physA && physA->entity) && (physB && physB->entity)) {
	//	Entity* entityA = physA->entity;
	//	Entity* entityB = physB->entity;

	//	// Puedes hacer casting si necesitas
	//	if (entityA->type == EntityType::PLAYER) {
	//		player = entityA;
	//		other = entityB;
	//	}
	//	else {
	//		player = entityB;
	//		other = entityA;
	//	}
	//}

	//if (physB && physB->entity) {
	//	Entity* entityB = physB->entity;
	//	LOG("Entity B type: %d", (int)entityB->GetType());
	//}


	// Obtener las entidades desde los PhysBody
	//Entity* entityA = reinterpret_cast<Entity*>(physA->body->GetUserData().pointer);
	//Entity* entityB = reinterpret_cast<Entity*>(physB->body->GetUserData().pointer);

	Entity* entityA = physA->entity;
	Entity* entityB = physB->entity;

	if (entityA->type == EntityType::PLAYER) {
		player = entityA;
		other = entityB;
	}
	else if(entityB->type == EntityType::PLAYER) {
		player = entityB;
		other = entityA;
	}

	// depending with which things the player has collided
	switch (other->type) {
		{
	case EntityType::TURBO_ON_ROAD:
		std::cout << "COLLISION TURBO" << std::endl;
		Player* p = dynamic_cast<Player*>(player);
		p->SetIsBoosted(true);
		p->SetTurboPower(1.0f);
		p->SetMaxSpeed(15.0f);

		break;
		}

	case EntityType::ROCK:

		break;
	default:
		break;
	}
}

void ModuleGame::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	//Entity* entityA = reinterpret_cast<Entity*>(physA->body->GetUserData().pointer);
	//Entity* entityB = reinterpret_cast<Entity*>(physB->body->GetUserData().pointer);

	//if (!entityA || !entityB) return;

	//LOG("COLLISION END: %d vs %d", (int)entityA->GetType(), (int)entityB->GetType());
}

void ModuleGame::GetPokemonChoosenByPlayer()
{
	//ask scene manager or character selection scene which pokemon has been choosed
}

#pragma region LEVEL CREATION
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
	for (int i = 0; i < 2; i++) {

#pragma region UNIMPLEMENTED SYSTEM OF CHOOSING CHARACTERS AND CREATION
		////Entity* racer;
		//if (i == 0) {

		//	switch (//choosenPokemon):
		//		{
		//		case //1:
		//			//racer = create cleffa
		//			break;
		//		case //2:
		//			//racer = create chansey
		//			break;
		//		case //3:
		//			//racer = create pachirisu
		//			break;
		//		case //4:
		//			//racer = create meganium
		//			break;
		//		default:
		//			break;
		//		}
		// 		//currentMap->AddChosenCharacter(choosenPokemon)
		//		//Player = racer; (change variable player (Player* -> Entity*))
		//		//racer->SetIsPlayer(true);

		//}
		//else {
		//	//randonmly check choosenPokemonList in level 1 and randomnly choose an unchosen number and create the corresponding pokemon
		//	int randomPokemon = GetRandomUnchosenPokemon(currentMap->GetPokemonsTakenList());

		//	switch (//int randomPokemon):
		//		{
		//		case //1:
		//			//racer = create cleffa
		//			break;
		//		case //2:
		//			//racer = create chansey
		//			break;
		//		case //3:
		//			//racer = create pachirisu
		//			break;
		//		case //4:
		//			//racer = create meganium
		//			break;
		//		default:
		//			break;
		//		}
		// 
		//		//currentMap->AddChosenCharacter(randomPokemon)
		//		//racer->SetIsPlayer(false);
		//		//racer->type == EntityType::AI;
		//		//FOR DOING THE NEXT TWO THINGS A DYNAMYC CAST IS NEEDED
		//		//racer->WaypointLoader("Assets/WaypointsAI.txt");
		//		//racer->SetMaxSpeed(8.0f);


		//}

		////racers.push_back(racer)
		////entityManager->AddEntity(racer);
#pragma endregion

		Player* racer = new Player(
			this,
			Vector2D(0.0f, 0.0f),  // Posición temporal
			EntityType::PLAYER,
			CARS,
			DEFAULT | SENSORS
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
			racer->WaypointLoader("Assets/WaypointsAI.txt");
			racer->SetMaxSpeed(8.0f);
			racer->type == EntityType::AI;
		}
	}

	// Configurar la cámara para seguir al jugador
	if (camera) {
		camera->SetTarget(player->GetCenter());
	}

	LOG("Created %d racers", racers.size());
}

int ModuleGame::GetRandomUnchosenPokemon(const std::vector<int>& chosenList)
{
	while (true) {
		int number = 1 + (rand() % 4); // entre 1 y 4

		// comprobar si ya está escogido
		bool alreadyChosen = false;
		for (int n : chosenList) {
			if (n == number) {
				alreadyChosen = true;
				break;
			}
		}

		if (!alreadyChosen)
			return number;
	}
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
		if (racer->GetPhysBody() && racer->GetPhysBody()->body) {
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
#pragma endregion


