#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class SceneManager : private Manager {
	friend class Engine;
private:
	Map<int, Scene*> scenes;
	Scene* currentScene;

	SceneManager(Managers* managers) : Manager(managers), currentScene(nullptr) {}
	~SceneManager() override;
	void destroy() override;
public:
	void add(int idx, Scene* scene);
	void load(int idx);
	Scene* getCurrentScene();

	void update(float dt);
	void fixedUpdate(float fixed_dt);
	void draw();
};