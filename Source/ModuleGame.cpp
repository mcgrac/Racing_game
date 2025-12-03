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
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//create level
	currentMap = new Level1(App->physics, this);
	currentMap->Start();

	return ret;
}


// Update: draw background
update_status ModuleGame::Update()
{
	currentMap->Update();
	return UPDATE_CONTINUE;
}

// Unload assets
bool ModuleGame::CleanUp()
{
	currentMap->CleanUp();
	delete currentMap;
	LOG("Unloading Intro scene");

	return true;
}
