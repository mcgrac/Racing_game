#pragma once
#include "raylib.h"
#include <vector>
#include "Button.h"
#include "Scene.h"

class GameOverScreen : public Scene {
public:
	GameOverScreen();
	void Start();
	void Draw();
	int Id;
	std::vector<Button*> buttons;
};
