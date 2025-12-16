#pragma once

#include "raylib.h"
#include <vector>
#include "Button.h"
#include "scene.h"

class GameScreen :public Scene {
protected:
	//TEXTURES
	Texture2D backg;
	Texture2D Frontg;

	void LoadTextures();

	//sounds

	void LoadSounds();

public:
	GameScreen();
	void Start();
	void Draw();
	void UnloadAssets();
	int Id;
	//std::vector<Button> buttons;
};
