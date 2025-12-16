#pragma once
#include "raylib.h"
#include <vector>
#include "Button.h"
#include "scene.h"

class GameOverScreen : public Scene {
protected:
	Texture2D backg;
	Texture2D victoryTitle;
	Texture2D defeatTitle;

	void LoadTextures();

	Sound buttonPressed;
	Sound buttonSelected;

	void LoadSounds();
public:
	GameOverScreen();
	void Start();
	void Update();
	void Draw();
	void UnloadAssets();
	int Id;
	bool victory;
	//std::vector<Button*> buttons;
};
