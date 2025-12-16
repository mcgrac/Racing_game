#pragma once

#include "raylib.h"
#include <vector>
#include "GameScreen.h"

//#include "raylib.h"

GameScreen::GameScreen() {

}

void GameScreen::LoadTextures() {
	/*backg = LoadTexture("Assets/Textures/Map/Background.png");
	manager->mdlGame->currentMap->RenderBackground();
	Frontg = LoadTexture("Assets/Textures/Map/TopElementsMap.png");*/
}
void GameScreen::LoadSounds() {

}
void GameScreen::Start() {
	//load all the textures and sounds
	LoadTextures();
	LoadSounds();
}
void GameScreen::Draw() {
	//DrawTexture(backg, 0, 0, WHITE);
}
void GameScreen::UnloadAssets() {
	//UnloadTexture(backg);
	//UnloadTexture(Frontg);
}

void GameScreen::Update() {

}