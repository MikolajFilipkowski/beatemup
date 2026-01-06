#include "scene_manager.h"

SceneManager::~SceneManager()
{
	destroy();
}

void SceneManager::destroy()
{
	for (auto& scene : scenes) {
		delete scene.value;
		scene.value = nullptr;
	}
	scenes.clear();
}

void SceneManager::changeScene()
{
	if (nextSceneIdx == 0) return;

	Scene* scene = scenes.get(nextSceneIdx);
	if (scene != nullptr) {
		if (currentScene != nullptr)
			currentScene->destroy();

		nextSceneIdx = 0;

		currentScene = scene;
		currentScene->start();
	}
}

void SceneManager::add(int idx, Scene* scene)
{
	if (scene == nullptr) return;

	scenes.put(idx, scene);
}

void SceneManager::load(int idx, bool instant)
{
	nextSceneIdx = idx;

	if (instant) changeScene();
}

Scene* SceneManager::getCurrentScene()
{
	return currentScene;
}

void SceneManager::update(float dt)
{
	if (nextSceneIdx != 0)
		changeScene();
	currentScene->update(dt);
}

void SceneManager::fixedUpdate(float fixed_dt)
{
	if (nextSceneIdx != 0)
		changeScene();
	currentScene->fixedUpdate(fixed_dt);
}

void SceneManager::draw()
{
	currentScene->draw();
}
