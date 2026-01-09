#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class ObjectManager : private Manager {
	friend class Engine;
private:
	ArrayList<GameObject*> objectArray;
	Map<int, ActionData*> actionsMap;

	ObjectManager(Managers* managers) : Manager(managers) {}
	~ObjectManager() override;
	void destroy() override;
	void startIfNeeded(GameObject*& object);
public:
	void add(GameObject* object);
	void remove(GameObject* object);

	void addAction(int key, ActionData* action);
	ActionData* getAction(int key);

	void updateAll(float dt);
	void fixedUpdateAll(float fixed_dt);
	void drawAll();
	void refreshObjects();
	void clear();
};