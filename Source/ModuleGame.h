#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

#include "Level1.h"
#include "GameCamera.h"
#include "PositionTracker.h"
#include "SceneManager.h"

#include "Cleffa.h"
#include "Meganium.h"
#include "Chansey.h"
#include "Pachirisu.h"

class PhysBody;
class PhysicEntity;
class EntityManager;
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
	Entity* GetPlayer() const { return player; }

	void OnCollision(PhysBody* physA, PhysBody* physB) override;
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) override;

	enum class RaceState {
		COUNTDOWN,
		RUNNING,
		FINISHED
	};

	enum class GameState {
		MAIN_MENU,
		CHARACTER_SELECTION,
		IN_GAME,
		FINISH
	};

private:

	void LoadLevel(int levelNumber);
	void CreatePlayers();
	int GetRandomUnchosenPokemon(const std::vector<int>& chosenList);
	void PositionPlayersOnGrid();

	Map* currentMap = nullptr;

	//std::vector<Player*> racers;
	std::vector<Entity*> racers;
	Entity* player = nullptr;
	int choosenPokemon; //hardcode chansey selection

	GameCamera* camera = nullptr;
	PositionTracker* posTracker = nullptr;
	EntityManager* entityManager = nullptr;
	SceneManager* sceneManager = nullptr;

	RaceState raceState;
	float countdownTimer;
	GameState gameState;

	int sceneState;
	int playerPosition;

	//scores
	float currentScore;
	float highestScore;
	float scoreWhenFinishingRace;

	//textures scenes
	Texture2D mainMenuScreen;
	Texture2D characterSelection;
	Texture2D finishRaceScreen;
};
