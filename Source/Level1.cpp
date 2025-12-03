#include "Level1.h"

Level1::Level1(ModulePhysics* physics, Module* _listener)
{
}

Level1::~Level1()
{
}

void Level1::Start()
{
	floor = LoadTexture("Assets/Textures/Map/Background.png");
	overMap = LoadTexture("Assets/Textures/Map/TopElementsMap.png");
}

void Level1::Update()
{
	//draw Basic map
	DrawTexture(floor, 0, 0, WHITE);
	DrawTexture(overMap, 0, 0, WHITE);
}

void Level1::CleanUp()
{
}
