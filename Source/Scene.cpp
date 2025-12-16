#include "raylib.h"
#include <vector>
#include "Scene.h"

void Scene::ChangeScene(GameSceneState nextScene) {
	manager->sceneChange(nextScene);
}

