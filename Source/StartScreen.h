#pragma once

#include "raylib.h"
#include <vector>
#include "Button.h"

enum class StartMenuSelectedButton {
	PLAY,
	HELP,
	BADGES,
	QUIT
};

class StartScreen:public Scene {

protected:
	//textures
	//background
	Texture2D backg;
	Texture2D Title;
	Texture2D Arrow;

	//play button
	Texture2D unpressed_play;
	Texture2D pressed_play;
	//quit button
	Texture2D unpressed_quit;
	Texture2D pressed_quit;
	//help button
	Texture2D unpressed_help;
	Texture2D pressed_help;
	//badges button
	Texture2D unpressed_badges;
	Texture2D pressed_badges;

	void LoadTextures();

	//sounds

	Sound buttonPressed;
	Sound buttonSelected;

	void LoadSounds();

	void UpdateArrowState();

public:
	StartScreen();
	void Start();
	void Update();
	void Draw();
	void UnloadAssets();
	int buttonId;
	StartMenuSelectedButton selButton;
	
};
