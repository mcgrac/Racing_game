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

	//create a character selection scene and add it
	s = new CharacterSelectionScreen();
	scenes.push_back(s);

	//create an ingame scene and add it to the list
	s = new GameScreen();
	scenes.push_back(s);

	//create and end screen
	s = new GameOverScreen();
	scenes.push_back(s);

	state = GameSceneState::MAIN_MENU;

}


void SceneManager::Update()
{

	//here check the enum game state scene to cange scene
	//Ex: if main menu render game menu scene, if character selectio render character selection
	//call update of the actual scene
}

void SceneManager::render() {
	//DRAW!!
	switch (state) {
	case GameSceneState::MAIN_MENU:
		scenes[0]->Draw();
		break;
	case GameSceneState::CHARACTER_SELECTION:
		scenes[1]->Draw();
		break;
	case GameSceneState::PLAYING:
		scenes[2]->Draw();
		break;
	case GameSceneState::FINISH_RACE:
		scenes[3]->Draw();
		break;
	}
}
