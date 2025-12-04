#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"


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

	//create player
	//entityManager->CreateEntity(EntityType::PLAYER);

	player = new Player(Vector2D(400.0f, 300.0f));

	//create level
	currentMap = new Level1(App->physics, this);
	currentMap->Start();

	camera = new GameCamera(SCREEN_WIDTH, SCREEN_HEIGHT);
	camera->SetSmoothSpeed(.15f);

	camera->CenterOn(player->GetCenter());

	return ret;
}


// Update: draw background
update_status ModuleGame::Update()
{
	float dt = GetFrameTime();
	entityManager->Update(dt);

	//std::cout << "MODULE GAME UPDATE" << std::endl;
	currentMap->Update();
	
	if(player)
	player->Update(dt);

	if(camera && player)
	{
		camera->FollowPlayer(player);
		camera->Update(dt);
	}
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModuleGame::CleanUp()
{
	if(currentMap)
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
