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
	int currSceneIdx;
	int nextSceneIdx;

	SceneManager(Managers* managers) 
		: Manager(managers), currentScene(nullptr), currSceneIdx(0), nextSceneIdx(0) {}
	~SceneManager() override;
	void destroy() override;
	void changeScene();
public:
	void add(int idx, Scene* scene);
	void load(int idx, bool instant = false);
	Scene* getCurrentScene();
	int getCurrentSceneIdx() const;

	void update(float dt);
	void fixedUpdate(float fixed_dt);
	void draw();
};