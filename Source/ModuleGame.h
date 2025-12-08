#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

#include"Level1.h"
#include "GameCamera.h"

class PhysBody;
class PhysicEntity;
class EntityManager;
class Player;        

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

	void OnCollision(PhysBody* physA, PhysBody* physB) override;
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) override;

private:

	Map* currentMap = nullptr;
	Player* player = nullptr;
	GameCamera* camera = nullptr;
	EntityManager* entityManager = nullptr;
};
