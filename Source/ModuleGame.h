#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

#include "Level1.h"
#include "GameCamera.h"
#include "PositionTracker.h"

#include "Cleffa.h"
#include "Meganium.h"
#include "Chansey.h"
#include "Pachirisu.h"

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
	update_status PreUpdate() override;
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	GameCamera* GetCamera() const { return camera; }
	Entity* GetPlayer() const { return player; }

	void OnCollision(PhysBody* physA, PhysBody* physB) override;
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) override;

	void GetPokemonChoosenByPlayer(); //in this void we will set the int choosenPokemon.

private:

	void LoadLevel(int levelNumber);
	void CreatePlayers();
	int GetRandomUnchosenPokemon(const std::vector<int>& chosenList);
	void PositionPlayersOnGrid();

	Map* currentMap = nullptr;

	//std::vector<Player*> racers;
	std::vector<Entity*> racers;
	Entity* player = nullptr;
	int choosenPokemon = 2; //hardcode chansey selection

	GameCamera* camera = nullptr;
	PositionTracker* posTracker = nullptr;
	EntityManager* entityManager = nullptr;
};
