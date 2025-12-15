#pragma once

#include "raylib.h"
#include <vector>
#include "Scene.h"
#include "GameScreen.h"
#include "Button.h"
#include "ModuleGame.cpp"
//#include "raylib.h"

void GameScreen::LoadTextures() {
	backg = LoadTexture("Assets/Textures/Map/Background.png"); //
	Frontg = LoadTexture("Assets/Textures/Map/TopElementsMap.png");
}
void GameScreen::LoadSounds() {

}
void GameScreen::Start() {
	//load all the textures and sounds
	LoadTextures();
	LoadSounds();
}
void GameScreen::Draw() {
	
}
void GameScreen::UnloadAssets() {
	UnloadTexture(backg);
	UnloadTexture(Frontg);
}