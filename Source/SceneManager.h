#pragma once

#include "raylib.h"
#include <vector>
#include "Scene.h"

class SceneManager {
	SceneManager();
	~SceneManager();

private:

	int currentScreenID();
	std::vector<Scene> scenes;

public:

	void Start();
	void Update();

	void SwitchScreen(int screenID);
	void MoveSelector();



};