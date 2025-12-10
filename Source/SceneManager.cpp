#include "SceneManager.h"
#include "StartScreen.h"

SceneManager::SceneManager() {

}
SceneManager::~SceneManager() {
	//empty vector
	scenes.clear();
	//destroy manager necessary?
}

void SceneManager::Start() {
	//get all the scenes and adding them to the vector
	Scene* s = new StartScreen();
}