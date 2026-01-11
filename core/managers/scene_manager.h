#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class SceneManager : private Manager {
	friend class Engine;
private:
	Map<int, Scene*> m_Scenes{};
	Scene* m_CurrentScene{};
	int m_CurrSceneIdx{ 0 };
	int m_NextSceneIdx{ 0 };

	SceneManager(Managers* a_Managers) 
		: Manager(a_Managers) {}
	~SceneManager() override;
	void destroy() override;
	void changeScene();
public:
	void add(int a_Key, Scene* a_Scene);
	void load(int a_Key, bool a_Instant = false);
	Scene* getCurrentScene();
	int getCurrentSceneIdx() const;

	void update(float a_Dt);
	void fixedUpdate(float a_FixedDt);
	void draw();
};