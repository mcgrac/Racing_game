//#pragma once

#include "raylib.h"
#include <vector>
#include "Button.h"

enum class characterSelection {
	CHANSEY,
	CLEFFA,
	PACHIRISU,
	MEGANIUM
};

class CharacterSelectionScreen : public Scene {
public:
	CharacterSelectionScreen();
	void Start();
	void Update();
	void Draw();
	void UnloadAssets();
	int Id;
	characterSelection charSelected;
protected:
	Texture2D SelectedChansey;
	Texture2D SelectedCleffa;
	Texture2D SelectedMeganium;
	Texture2D SelectedPachirisu;
	Texture2D Cursor;

	void LoadTextures();

	//button sounds
	Sound buttonPressed;
	Sound buttonSelected;

	void LoadSounds();

};