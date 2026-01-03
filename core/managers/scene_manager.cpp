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

void SceneManager::add(int idx, Scene* scene)
{
	if (scene == nullptr) return;

	scenes.put(idx, scene);
}

void SceneManager::load(int idx)
{
	Scene* scene = scenes.get(idx);
	if (scene != nullptr) {
		if (currentScene != nullptr)
			currentScene->destroy();

		currentScene = scene;
		currentScene->start();
	}
}

Scene* SceneManager::getCurrentScene()
{
	return currentScene;
}

void SceneManager::update(float dt)
{
	currentScene->update(dt);
}

void SceneManager::fixedUpdate(float fixed_dt)
{
	currentScene->fixedUpdate(fixed_dt);
}

void SceneManager::draw()
{
	currentScene->draw();
}
