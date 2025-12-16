#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include <random>

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

	//load textures scenes
	mainMenuScreen = LoadTexture("Assets/Textures/UI/Start/ExampleStart.png");
	characterSelection = LoadTexture("Assets/Textures/UI/CharacterSelect/Background.png");
	finishRaceScreen = LoadTexture("Assets/Textures/UI/GameOver(Temporary)/GameOverTemp.png");

	gameState = GameState::MAIN_MENU;
	choosenPokemon = -1;
	scoreWhenFinishingRace = 0.0f;
	//entityManager = new EntityManager();

	////load level
	//LoadLevel(1);
	//CreatePlayers();
	//PositionPlayersOnGrid();


	//canmera inicialization
	camera = new GameCamera(SCREEN_WIDTH, SCREEN_HEIGHT);
	//camera->SetSmoothSpeed(.15f);
	//camera->CenterOn(player->GetCenter());

	//call start entity manager -> call start of all entities
	//entityManager->Start();

	//create position tracking and send al checkpoints
	//posTracker = new PositionTracker(currentMap->GetCheckpointsList());

	//create scene managewr
	//sceneManager = new SceneManager();

	//raceState = RaceState::COUNTDOWN;
	//countdownTimer = 3.0f;
	return ret;
}

update_status ModuleGame::PreUpdate()
{
	App->physics->SetCameraDebug(camera->GetRaylibCamera());
	return UPDATE_CONTINUE;
}


// Update: draw background
update_status ModuleGame::Update()
{
	float dt = GetFrameTime();

	switch (gameState)
	{
	case ModuleGame::GameState::MAIN_MENU:
		if (IsKeyPressed(KEY_ONE)) {
			gameState = GameState::CHARACTER_SELECTION;
		}
		else if (IsKeyPressed(KEY_TWO)) {
			//quit
		}
		break;
		{
	case ModuleGame::GameState::CHARACTER_SELECTION:

		bool choosenCharacter = false;

		if (IsKeyPressed(KEY_ONE)) {
			choosenPokemon = 1;
			choosenCharacter = true;
		}
		else if (IsKeyPressed(KEY_TWO)) {
			choosenPokemon = 2;
			choosenCharacter = true;
		}
		else if (IsKeyPressed(KEY_THREE)) {
			choosenPokemon = 3;
			choosenCharacter = true;
		}
		else if (IsKeyPressed(KEY_FOUR)) {
			choosenPokemon = 4;
			choosenCharacter = true;
		}

		if (choosenCharacter)
		{
			//entity manager inicialization
			entityManager = new EntityManager();

			//camera inicialization
			//camera = new GameCamera(SCREEN_WIDTH, SCREEN_HEIGHT);
			//load level
			LoadLevel(1);
			CreatePlayers();
			PositionPlayersOnGrid();

			camera->SetSmoothSpeed(.15f);
			camera->CenterOn(player->GetCenter());



			entityManager->Start();

			posTracker = new PositionTracker(currentMap->GetCheckpointsList());

			//set race inicialization
			countdownTimer = 3.0f;
			raceState = RaceState::COUNTDOWN;

			currentScore = 0.0f;

			playerPosition = 0;

			//change to In-game state
			gameState = GameState::IN_GAME;
		}

		break;
		}
	case ModuleGame::GameState::IN_GAME:
		//game logic
		switch (raceState)
		{
			{
		case ModuleGame::RaceState::COUNTDOWN:
			std::cout << "State countodwns" << std::endl;
			//countdownTimer -= dt;

			static float startTime = GetTime(); // GetTime() devuelve segundos desde el inicio del juego
			float elapsed = GetTime() - startTime;
			float countdownDuration = countdownTimer;

			if (elapsed >= countdownDuration) {
				raceState = RaceState::RUNNING;
			}
			else {
				std::cout << "Countdown: " << countdownDuration - elapsed << "s remaining\n";
			}
			break;
			}
			{
		case ModuleGame::RaceState::RUNNING:
			std::cout << "State running" << std::endl;
			if (entityManager) { entityManager->Update(dt); }

			//update position of all racers
			posTracker->UpdatePositions(racers);

			currentScore = GetTime() - countdownTimer;
			std::cout << "Current Score: " << currentScore << " points" << std::endl;

			//check if player laps are 3 ->finish race
			for (Entity* racer : racers) {
				Characters* chara = dynamic_cast<Characters*>(racer);
				if (chara) {
					if (chara->GetLaps() == 3) {
						scoreWhenFinishingRace = currentScore;
						raceState = RaceState::FINISHED;
					}
				}
			}

			break;
			}
			{
		case ModuleGame::RaceState::FINISHED:

			//get the final position of the player
			Characters* chara = dynamic_cast<Characters*>(player);
			playerPosition = chara->GetPositionInRace();

			//go to game over scene and delete
			if (currentMap)
			{
				currentMap->CleanUp();
				delete currentMap;
				currentMap = nullptr;
			}

			if (entityManager)
			{
				delete entityManager;
				entityManager = nullptr;
			}

			if (posTracker)
			{
				delete posTracker;
				posTracker = nullptr;
			}

			if (player) {
				delete player;
				player = nullptr;

			}

			for (Entity* racer : racers) {
				racer->pendingToDelete = true;
				racer->active = false;

			}

			racers.clear();

			//update score if improved
			if (currentScore > highestScore) {
				highestScore = currentScore;
			}

			gameState = GameState::FINISH;
			break;
			}
		default:
			break;
		}

		//loop music level
		if(currentMap)
		{
			currentMap->UpdateMusic();
		}

		if (camera && player)
		{
			camera->FollowPlayer(player);
			camera->Update(dt);
		}

		break;
	case ModuleGame::GameState::FINISH:
		//game over screen. Show dcore and update if necessary

		if (IsKeyPressed(KEY_ONE)) {
			gameState = GameState::MAIN_MENU;
		}
		break;
	default:
		break;
	}

	return UPDATE_CONTINUE;
}

//render in post-update
update_status ModuleGame::PostUpdate()
{
	switch (gameState)
	{
	case ModuleGame::GameState::MAIN_MENU:
		//draw main menu texture
		DrawTexture(mainMenuScreen, 0, 0, WHITE);

		// Dibujar texto de instrucciones
		DrawText("Press 1 to Start the Game",
			SCREEN_WIDTH / 2 - 180,
			SCREEN_HEIGHT - 100,
			30,
			WHITE);

		break;
		{
	case ModuleGame::GameState::CHARACTER_SELECTION:
		DrawTexture(characterSelection, 0, 0, WHITE);

		DrawText("SELECT YOUR CHARACTER",
			SCREEN_WIDTH / 2 - 250,
			50,
			40,
			YELLOW);

		int startY = 180;
		int spacing = 120;

		// CLEFFA
		DrawText("Press 1 -> Cleffa",
			SCREEN_WIDTH / 2 - 250,
			startY,
			35,
			WHITE);
		DrawText("Ability: Become invisible and ignore colliders",
			SCREEN_WIDTH / 2 - 240,
			startY + 35,
			22,
			LIGHTGRAY);

		// CHANSEY
		DrawText("Press 2 -> Chansey",
			SCREEN_WIDTH / 2 - 250,
			startY + spacing,
			35,
			WHITE);
		DrawText("Ability: Throw an egg",
			SCREEN_WIDTH / 2 - 240,
			startY + spacing + 35,
			22,
			LIGHTGRAY);

		// PACHIRISU
		DrawText("Press 3 -> Pachirisu",
			SCREEN_WIDTH / 2 - 250,
			startY + spacing * 2,
			35,
			WHITE);
		DrawText("Ability: Electric discharge around",
			SCREEN_WIDTH / 2 - 240,
			startY + spacing * 2 + 35,
			22,
			LIGHTGRAY);

		// MEGANIUM
		DrawText("Press 4 -> Meganium",
			SCREEN_WIDTH / 2 - 250,
			startY + spacing * 3,
			35,
			WHITE);
		DrawText("Ability: Shoot laser beam from mouth",
			SCREEN_WIDTH / 2 - 240,
			startY + spacing * 3 + 35,
			22,
			LIGHTGRAY);

		break;
		}
	case ModuleGame::GameState::IN_GAME:
		//MouseJoint
		App->physics->UseMouseJoint(camera->GetRaylibCamera());

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
		break;
		{
	case ModuleGame::GameState::FINISH:

		DrawTexture(finishRaceScreen, 0, 0, WHITE);

		//draw text current score and highest score
		// Convertir los tiempos a minutos:segundos
		int currentMinutes = (int)scoreWhenFinishingRace / 60;
		int currentSeconds = (int)scoreWhenFinishingRace % 60;
		int currentMillis = (int)((scoreWhenFinishingRace - (int)scoreWhenFinishingRace) * 100);

		int bestMinutes = (int)highestScore / 60;
		int bestSeconds = (int)highestScore % 60;
		int bestMillis = (int)((highestScore - (int)highestScore) * 100);

		// Determinar sufijo ordinal (1st, 2nd, 3rd, 4th)
		const char* positionSuffix = "th";
		if (playerPosition == 1) positionSuffix = "st";
		else if (playerPosition == 2) positionSuffix = "nd";
		else if (playerPosition == 3) positionSuffix = "rd";

		// Color según posición
		Color positionColor = GRAY;
		if (playerPosition == 1) positionColor = GOLD;
		else if (playerPosition == 2) positionColor = LIGHTGRAY;
		else if (playerPosition == 3) {
			positionColor.r = 205;
			positionColor.g = 127;
			positionColor.b = 50;
			positionColor.a = 255;
		}; // Bronce

		// Dibujar título
		DrawText("RACE FINISHED!",
			SCREEN_WIDTH / 2 - 200,
			100,
			60,
			YELLOW);

		// Dibujar tiempo actual (esta carrera)
		DrawText("YOUR TIME:",
			SCREEN_WIDTH / 2 - 150,
			250,
			40,
			WHITE);

		DrawText(TextFormat("%02d:%02d.%02d", currentMinutes, currentSeconds, currentMillis),
			SCREEN_WIDTH / 2 - 100,
			320,
			50,
			GREEN);

		// Dibujar mejor tiempo
		if (highestScore > 0.0f) {
			DrawText("BEST TIME:",
				SCREEN_WIDTH / 2 - 150,
				450,
				40,
				WHITE);

			DrawText(TextFormat("%02d:%02d.%02d", bestMinutes, bestSeconds, bestMillis),
				SCREEN_WIDTH / 2 - 100,
				520,
				50,
				GOLD);

			// Mensaje si es nuevo récord
			if (scoreWhenFinishingRace <= highestScore) {
				DrawText("NEW RECORD!",
					SCREEN_WIDTH / 2 - 150,
					600,
					35,
					RED);
			}
		}

		// Instrucciones
		DrawText("Press 1 to return to Main Menu",
			SCREEN_WIDTH / 2 - 200,
			SCREEN_HEIGHT - 100,
			25,
			LIGHTGRAY);
		break;
		}
	default:
		break;
	}


	//--------------------------------------
	//---------UI debug render--------------
	//Show position of the player
	if (player && raceState == RaceState::RUNNING) {

		Characters* playerCar = dynamic_cast<Characters*>(player);

		// 1. Obtener los datos relevantes
		int currentPosition = playerCar->GetPositionInRace();
		int currentLap = playerCar->GetLaps() + 1; // Asumo que `laps` es vueltas completadas.

		// 2. Construir el texto a mostrar
		// Ejemplo: "POS: 1º / VUELTA: 2"
		std::string debugText = "POS: ";
		// Convertir la posición a cadena (con indicador ordinal si es necesario)
		if (currentPosition > 0) {
			// Se puede usar la lógica del PositionTracker para generar el ordinal (1º, 2º, 3º, etc.)
			debugText += std::to_string(currentPosition) + (currentPosition == 1 ? "º" : " ");
		}
		else {
			debugText += "? "; // Si aún no se ha calculado
		}

		debugText += " / Lap: " + std::to_string(currentLap);

		DrawText(
			debugText.c_str(),
			800, 10, 40, WHITE );
	}
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
		currentMap = nullptr;
	}

	if (player) {
		delete player;
		player = nullptr;
	}

	if (camera) {
		delete camera;
		camera = nullptr;
	}

	if (posTracker) {
		delete posTracker;
		posTracker = nullptr;
	}

	if (!racers.empty()) {
		for (Entity* e : racers) {
			e->pendingToDelete = true;
			e->active = false;
		}
		racers.clear();
	}

	LOG("Unloading Intro scene");

	return true;
}

void ModuleGame::OnCollision(PhysBody* physA, PhysBody* physB) {
	if (gameState != GameState::IN_GAME && raceState != RaceState::RUNNING) { return; }
	if (!physA->entity || !physB->entity) { return; }

	Entity* player = nullptr;
	Entity* other = nullptr;

	Entity* entityA = physA->entity;
	Entity* entityB = physB->entity;

	//if the entity is the player
	if (entityA->type == EntityType::PLAYER || entityA->type == EntityType::AI) {
		player = entityA;
		other = entityB;
	}
	else if(entityB->type == EntityType::PLAYER || entityB->type == EntityType::AI) {
		player = entityB;
		other = entityA;
	}

	// depending with which things the player has collided
	switch (other->type) {
		{
	case EntityType::TURBO_ON_ROAD:
		Characters* c = dynamic_cast<Characters*>(player);
		c->SetIsBoosted(true);
		c->SetTurboPower(1.0f);
		c->SetMaxSpeed(15.0f);
		break;
		}
		{
	case EntityType::ROCK:
		Interactables* intera = dynamic_cast<Interactables*>(other);
		PlaySound(intera->GetDestructionSound());
		intera->SetIsDestroyed(true);
		break;
		}
		{
	case EntityType::CHECKPOINT:
		//std::cout << "Collision Checkpoint" << std::endl;
		Checkpoint* c = dynamic_cast<Checkpoint*>(other); //get id from checkpoint entity
		Characters* chara = dynamic_cast<Characters*>(player); //set id checkpoint to player

		//if the checkpoint is the first one and the one before is the last one means 1 lap has been completed
		if (chara->GetCheckId() >= 10 && c->GetId() == 0) { 
			chara->AddOneLap(); 
		}

		chara->SetCheckpointArrived(c->GetId()); //update last checkpoint racers
		break;
		}
		{
	case EntityType::ATTACK:
		//collision AI with a attack of the player ->set AI stunned for a while
		std::cout << "attackCollision" << std::endl;
		Characters* chara = dynamic_cast<Characters*>(player);
		if(chara->GetCurrentState() != 2)
		{
			chara->SetStunnedState();
			chara->SetStateTimer(2.0f); //be stunned for 2 seconds
		}
		break;
		}
		{
	case EntityType::OFF_ROAD:
		std::cout << "Off road enter" << std::endl;
		Characters* chara = dynamic_cast<Characters*>(player);
		if (chara->GetCurrentState() != 2 && !chara->GetIsOffRoad()) { //if not stunned
			std::cout << "Max speed before entering: " << chara->GetMaxSpeed() << std::endl;
			chara->SetIsOffRoad(true);
			chara->SetMaxSpeed(chara->GetMaxSpeed() / 2.0f);
			std::cout << "Max speed after entering: " << chara->GetMaxSpeed() << std::endl;
		}
		break;
		}
	default:

		break;
		
	}
}

void ModuleGame::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	if (gameState != GameState::IN_GAME && raceState != RaceState::RUNNING) { return; }
	if (!physA->entity || !physB->entity) { return; }

	Entity* player = nullptr;
	Entity* other = nullptr;

	Entity* entityA = physA->entity;
	Entity* entityB = physB->entity;

	//if the entity is the player
	if (entityA->type == EntityType::PLAYER || entityA->type == EntityType::AI) {
		player = entityA;
		other = entityB;
	}
	else if (entityB->type == EntityType::PLAYER || entityB->type == EntityType::AI) {
		player = entityB;
		other = entityA;
	}

	switch (other->type)
	{
		{
	case EntityType::OFF_ROAD:
		std::cout << "Off road exit" << std::endl;
		Characters* chara = dynamic_cast<Characters*>(player);
		if (chara->GetIsOffRoad()) {
			std::cout << "Max speed before exiting: " << chara->GetMaxSpeed() << std::endl;
			chara->SetIsOffRoad(false);
			chara->SetMaxSpeed(chara->GetMaxSpeed() * 2.0f);
			std::cout << "Max speed after exiting: " << chara->GetMaxSpeed() << std::endl;
		}
		break;
		}
	default:
		break;
	}
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
	for (int i = 0; i < 4; i++) {

	    Entity* racer = nullptr;
		if (i == 0) {
			switch (choosenPokemon){
				case 1:
					racer = new Cleffa(
						this,
						Vector2D(0.0f, 0.0f),
						EntityType::PLAYER,
						PhysicCategory::CARS,
						PhysicCategory::SENSORS | WALLS | DESTRUCTIBLE | CHECKPOINTS
					);
					break;
				case 2:
					racer = new Chansey(
						this,
						Vector2D(0.0f, 0.0f),
						EntityType::PLAYER,
						PhysicCategory::CARS,
						PhysicCategory::SENSORS | WALLS | DESTRUCTIBLE | CHECKPOINTS
					);
					break;
				case 3:
					racer = new Pachirisu(
						this,
						Vector2D(0.0f, 0.0f),
						EntityType::PLAYER,
						PhysicCategory::CARS,
						PhysicCategory::SENSORS | WALLS | DESTRUCTIBLE | CHECKPOINTS
					);
					break;
				case 4:
					racer = new Meganium(
						this,
						Vector2D(0.0f, 0.0f),
						EntityType::PLAYER,
						PhysicCategory::CARS,
						PhysicCategory::SENSORS | WALLS | DESTRUCTIBLE | CHECKPOINTS
					);
					break;
				default:
					break;
			}

			currentMap->AddChosenCharacter(choosenPokemon);
			player = racer; /*(change variable player (Player* -> Entity*))*/
			racer->SetIsPlayer(true);

		}
		else {

			//randonmly check choosenPokemonList in level 1 and randomnly choose an unchosen number and create the corresponding pokemon
			int randomPokemon = GetRandomUnchosenPokemon(currentMap->GetPokemonsTakenList());

			std::cout << "Random choosed: " << randomPokemon << std::endl;

			switch (randomPokemon){
				case 1:
					racer = new Cleffa(
						this,
						Vector2D(0.0f, 0.0f),
						EntityType::AI,
						PhysicCategory::AI,
						PhysicCategory::CHECKPOINTS | ATTACK
					);
					break;
				case 2:
					racer = new Chansey(
						this,
						Vector2D(0.0f, 0.0f),
						EntityType::AI,
						PhysicCategory::AI,
						PhysicCategory::CHECKPOINTS | ATTACK
					);
					break;
				case 3:
					racer = new Pachirisu(
						this,
						Vector2D(0.0f, 0.0f),
						EntityType::AI,
						PhysicCategory::AI,
						PhysicCategory::CHECKPOINTS | ATTACK
					);
					break;
				case 4:
					racer = new Meganium(
						this,
						Vector2D(0.0f, 0.0f),
						EntityType::AI,
						PhysicCategory::AI,
						PhysicCategory::CHECKPOINTS | ATTACK
					);
					break;
				default:
					break;
			}
		 
			currentMap->AddChosenCharacter(randomPokemon);
			racer->SetIsPlayer(false);
			
			Characters* c = dynamic_cast<Characters*>(racer);
			c->WaypointLoader("Assets/WaypointsAI.txt");
			c->SetMaxSpeed(8.0f);


		}

		racers.push_back(racer);
		entityManager->AddEntity(racer);
	}

	// Configurar la cámara para seguir al jugador
	if (camera) {
		camera->SetTarget(player->GetCenter());
	}

	LOG("Created %d racers", racers.size());
}
int ModuleGame::GetRandomUnchosenPokemon(const std::vector<int>& chosenList)
{

	// Generador aleatorio (se inicializa con semilla basada en tiempo)
	static std::random_device rd; // semilla real
	static std::mt19937 gen(rd()); // Mersenne Twister

	// Crear lista de números posibles que no estén escogidos
	std::vector<int> availableNumbers;
	for (int i = 1; i <= 4; ++i) {
		if (std::find(chosenList.begin(), chosenList.end(), i) == chosenList.end()) {
			availableNumbers.push_back(i);
		}
	}

	if (availableNumbers.empty())
		return -1; // todos los Pokémon ya están elegidos

	// Elegir un índice aleatorio de los disponibles
	std::uniform_int_distribution<> dis(0, static_cast<int>(availableNumbers.size() - 1));
	return availableNumbers[dis(gen)];
}
void ModuleGame::PositionPlayersOnGrid()
{
	const std::vector<Vector2D>& gridPositions = currentMap->GetStartingGrid();

	// Posicionar cada coche en su posición de la parrilla
	for (size_t i = 0; i < racers.size() && i < gridPositions.size(); i++) {
		Entity* racer = racers[i];
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


