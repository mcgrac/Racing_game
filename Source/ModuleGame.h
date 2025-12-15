#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

#include"Level1.h"
#include "GameCamera.h"

#include"SceneManager.h"

class PhysBody;
class PhysicEntity;
class EntityManager;
class Player;  
class SceneManager;

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
	Player* GetPlayer() const { return player; }

	void OnCollision(PhysBody* physA, PhysBody* physB) override;
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) override;

	void GetPokemonChoosenByPlayer(); //in this void we will set the int choosenPokemon.

private:

	void LoadLevel(int levelNumber);
	void CreatePlayers();
	int GetRandomUnchosenPokemon(const std::vector<int>& chosenList);
	void PositionPlayersOnGrid();

	Map* currentMap = nullptr;

	std::vector<Player*> racers;
	Player* player = nullptr;
	int choosenPlayer;

	GameCamera* camera = nullptr;

	EntityManager* entityManager = nullptr;

	SceneManager* sceneManager = nullptr;

	//testing
	Player* playerTesting = nullptr;
};
