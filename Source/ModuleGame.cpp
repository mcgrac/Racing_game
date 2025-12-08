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

	//player = new Player(Vector2D(400.0f, 300.0f));
	//creation entities
	entityManager->CreateEntity(EntityType::PLAYER);

	//create level
	currentMap = new Level1(App->physics, this);
	currentMap->Start();

	//canmera inicialization
	camera = new GameCamera(SCREEN_WIDTH, SCREEN_HEIGHT);
	camera->SetSmoothSpeed(.15f);
	camera->CenterOn(entityManager->GetPlayer()->GetCenter());

	//call start entity manager -> call start of all entities
	entityManager->Start();
	return ret;
}


// Update: draw background
update_status ModuleGame::Update()
{
	float dt = GetFrameTime();

	if (entityManager) { entityManager->Update(dt); }
	else { std::cout<<"Entity manager update error Module Game\n"; }

	//std::cout << "MODULE GAME UPDATE" << std::endl;
	//currentMap->Update();

	/*if (player) { player->Update(dt); }
	else { LOG("Player error Module Game\n"); }*/


	if (camera && entityManager->GetPlayer())
	{
		camera->FollowPlayer(entityManager->GetPlayer());
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
	if (entityManager) { entityManager->Render(); }
	else { std::cout << "Entity manager render error Module Game\n"; }
	//render top elements
	if (currentMap) { currentMap->RenderTop(); }

	EndMode2D();
	//--------------------------------------
	//---------UI debug render--------------


	//--------------------------------------
	return UPDATE_CONTINUE;;
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
