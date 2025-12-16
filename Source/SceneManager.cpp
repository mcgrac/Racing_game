#include "SceneManager.h"
#include "Scene.h"
#include "StartScreen.h"
#include "CharacterSelectionScreen.h"
#include "GameScreen.h"
#include "GameOverScreen.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	scenes.clear();
}

void SceneManager::Start()
{
	//create a start screen and add it to the list 
	Scene* s = new StartScreen();
	scenes.push_back(s);
	scenes[0]->manager = this;

	//create a character selection scene and add it
	s = new CharacterSelectionScreen();
	scenes.push_back(s);
	scenes[1]->manager = this;

	//create an ingame scene and add it to the list
	s = new GameScreen();
	scenes.push_back(s);
	scenes[2]->manager = this;

	//create and end screen
	s = new GameOverScreen();
	scenes.push_back(s);
	scenes[3]->manager = this;

	state = GameSceneState::MAIN_MENU;
	indexScene = 0;

}


void SceneManager::Update()
{

	//here check the enum game state scene to cange scene
	//Ex: if main menu render game menu scene, if character selectio render character selection
	//call update of the actual scene
	if (targetScene != indexScene) {//target scene has been changed

	}
}

void SceneManager::render() {
	//DRAW!!
	switch (state) {
	case GameSceneState::MAIN_MENU:
		indexScene = 0;
		scenes[0]->Draw();
		break;
	case GameSceneState::CHARACTER_SELECTION:
		indexScene = 1;
		scenes[1]->Draw();
		break;
	case GameSceneState::PLAYING:
		indexScene = 2;
		scenes[2]->Draw();
		break;
	case GameSceneState::FINISH_RACE:
		indexScene = 3;
		scenes[3]->Draw();
		break;
	}
}

void SceneManager::sceneChange(GameSceneState newState) {
	scenes[indexScene]->UnloadAssets();
	state = newState;
}
