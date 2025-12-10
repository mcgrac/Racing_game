#pragma once

#include "raylib.h"
#include <vector>

class SceneManager {

private:

	int currentScreenID();

public:

	void Start();
	void Update();

	void SwitchScreen(int screenID);
	void MoveSelector();



};