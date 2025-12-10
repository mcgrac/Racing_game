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

private:

	int currentScreenID();

public:

	void Start();
	void Update();

	void SwitchScreen(int screenID);
	void MoveSelector();



};