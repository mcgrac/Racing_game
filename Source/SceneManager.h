#pragma once

#include "raylib.h"
#include <vector>

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

	void Start();
	void Update();

	//void SwitchScreen(int screenID);
	//void MoveSelector();

	inline GameSceneState GetCurrentScene() { return currentScene; }

private:

	GameSceneState currentScene;


};