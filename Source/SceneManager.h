#pragma once

#include "raylib.h"
#include <vector>
#include "Scene.h"
#include "Module.h"

enum class GameSceneState {
	MAIN_MENU,
	PLAYING,
	FINISH_RACE,
	CHARACTER_SELECTION
};

class SceneManager {
public:
	SceneManager();
	~SceneManager();

private:

	int currentScreenID();
	std::vector<Scene*> scenes;
	int indexScene;

public:

	SceneManager();
	~SceneManager();

	void sceneChange(GameSceneState newState);

	void Start();
	void Update();
	void render();

	//void SwitchScreen(int screenID);
	//void MoveSelector();

	GameSceneState state;

	int targetScene = 0;


};