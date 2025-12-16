#include "GameOverScreen.h"

GameOverScreen::GameOverScreen() {

}

void GameOverScreen::LoadTextures() {
	backg = LoadTexture("Assets/Textures/UI/GameOver/GameOverBack.png");
}

void GameOverScreen::LoadSounds() {

}

void GameOverScreen:: Start() {
	LoadTextures();
	LoadSounds();
}

void GameOverScreen::Update() {

}

void GameOverScreen::Draw() {
	DrawTexture(backg, 0, 0, WHITE);
}

void GameOverScreen::UnloadAssets() {
	UnloadTexture(backg);
}

