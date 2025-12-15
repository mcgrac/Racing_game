#pragma once

#include "raylib.h"
#include <vector>
#include "Button.h"
#include "SceneManager.h"

class Scene {

private:
	Scene(int Id);
protected:
	//textures
	void LoadTextures();

	//sounds
	void LoadSounds();

public:

	void Start();
	void Update();

	void startTrasition();
	void UnloadAssets();

	void Draw();

	std::vector<Button*> buttons;

	SceneManager manager;

};
