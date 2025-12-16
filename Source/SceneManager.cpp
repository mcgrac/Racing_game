#include "SceneManager.h"


SceneManager::SceneManager()
{
	currentScene = GameSceneState::MAIN_MENU;
}

SceneManager::~SceneManager()
{
	scenes.clear();
}

void SceneManager::Start()
{
	//create a start screen and add it to the list 
	Scene* s0 = new StartScreen();
	scenes.push_back(s0);

	//create a character selection scene and add it
	Scene* s1 = new CharacterSelectionScreen();
	scenes.push_back(s1);

	//create an ingame scene and add it to the list
	Scene* s2 = new GameScreen();
	scenes.push_back(s2);

	//create and end screen
	Scene* s3 = new GameOverScreen();
	scenes.push_back(s3);

	state = GameSceneState::MAIN_MENU;
	indexScene = 0;
	scenes[indexScene]->Start();

}


void SceneManager::Update()
{

	//here check the enum game state scene to cange scene
	//Ex: if main menu render game menu scene, if character selectio render character selection
	//call update of the actual scene
	scenes[indexScene]->Update();
	render();
	int aux = indexScene;
	if (scenes[indexScene]->GetTargetScene() != -1) {
		indexScene = scenes[indexScene]->GetTargetScene();
		scenes[aux]->targetScene = -1;
		//change state based on the new index
		switch (indexScene) {
		case 0:
			state = GameSceneState::MAIN_MENU;
			break;
		case 1:
			state = GameSceneState::CHARACTER_SELECTION;
			break;
		case 2:
			startedPlaying = true;
			GameSceneState::PLAYING;
			break;
		case 3:
			GameSceneState::FINISH_RACE;
			break;
		case 4:
			//close game = true;
			break;
		}
		if (indexScene <= 3) {
			scenes[indexScene]->Start();
		}
	}

	
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

