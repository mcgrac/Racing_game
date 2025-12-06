#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

#include"Map.h"
#include"Level1.h"
#include "GameCamera.h"

#include "EntityManager.h"

class PhysBody;
class PhysicEntity;


class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	GameCamera* GetCamera() const { return camera; }
	Player* GetPlayer() const { return player; }

private:

	Map* currentMap = nullptr;
	Player* player = nullptr;
	GameCamera* camera = nullptr;
	EntityManager* entityManager = nullptr;
};
