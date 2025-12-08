#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
//#include "EntityManager.h"

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

	//entityManager = new EntityManager();

	//CREATION ENTITIES
	//Player
	//entityManager->CreateEntity(EntityType::PLAYER);
	//Ai
	

	
	//create level
	currentMap = new Level1(App->physics, this, App->entity_manager);
	currentMap->Start();

	//canmera inicialization
	camera = new GameCamera(SCREEN_WIDTH, SCREEN_HEIGHT);
	camera->SetSmoothSpeed(.15f);
	camera->CenterOn(App->entity_manager->GetPlayer()->GetCenter());

	//call start entity manager -> call start of all entities
	//entityManager->Start();
	return ret;
}


// Update: draw background
update_status ModuleGame::Update()
{
	float dt = GetFrameTime();

	//if (entityManager) { entityManager->Update(dt); }
	//else { std::cout<<"Entity manager update error Module Game\n"; }

	//std::cout << "MODULE GAME UPDATE" << std::endl;
	currentMap->Update();

	/*if (player) { player->Update(dt); }
	else { LOG("Player error Module Game\n"); }*/


	if (camera && App->entity_manager->GetPlayer())
	{
		camera->FollowPlayer(App->entity_manager->GetPlayer());
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
	App->entity_manager->Render();
	//render top elements
	if (currentMap) { currentMap->RenderTop(); }

	//EndMode2D();
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
	// Obtener las entidades desde los PhysBody
	Entity* entityA = reinterpret_cast<Entity*>(physA->body->GetUserData().pointer);
	Entity* entityB = reinterpret_cast<Entity*>(physB->body->GetUserData().pointer);

	if (!entityA || !entityB) {
		LOG("WARNING: Collision with null entity");
		return;
	}

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
	Entity* entityA = reinterpret_cast<Entity*>(physA->body->GetUserData().pointer);
	Entity* entityB = reinterpret_cast<Entity*>(physB->body->GetUserData().pointer);

	if (!entityA || !entityB) return;

	//LOG("COLLISION END: %d vs %d", (int)entityA->GetType(), (int)entityB->GetType());
}
