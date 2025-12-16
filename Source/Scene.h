#pragma once

#include "raylib.h"
#include <vector>
#include "Button.h"

class Scene {

protected:
	//textures
	virtual void LoadTextures();

	//sounds
	virtual void LoadSounds();

public:

	Scene();

	int targetScene = -1;

	virtual void Start();
	virtual void Update();

	int GetTargetScene();
	virtual void UnloadAssets();

	virtual void Draw();

	std::vector<Button*> buttons;

};
