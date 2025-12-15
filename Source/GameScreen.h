#pragma once

#include "raylib.h"
#include <vector>
#include "Button.h"

class GameScreen :public Scene {

public:
	GameScreen();
	void Start();
	void Draw();
	int Id;
	std::vector<Button> buttons;
};
