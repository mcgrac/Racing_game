#pragma once
#include"Globals.h"
#include "raylib.h"

class Map {

public:

	void Start();
	void Update();


protected:
	virtual void CreateLevel();

	Texture2D floor;
	Texture2D overMap;
private:

};
