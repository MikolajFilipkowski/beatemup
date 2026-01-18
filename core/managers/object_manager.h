#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class ObjectManager : private Manager {
	friend class Engine;
private:
	ArrayList<GameObject*> m_ObjectArray{};
	Map<int, ActionData*> m_ActionsMap{};
	float m_Gravity{ 0 };

	ObjectManager(Managers* a_Managers) : Manager(a_Managers) {}
	~ObjectManager() override;
	void destroy() override;
	void startIfNeeded(GameObject*& a_Object);
	void checkCollisions(int a_I, GameObject*& a_Obj1, Rigidbody& a_Rb1, Cuboid& a_Coll1);
public:
	void add(GameObject* a_Object);
	void remove(GameObject* a_Object);

	ArrayList<GameObject*>& getAllObjects();
	Map<int, ActionData*>& getAllActions();

	void addAction(int a_Key, ActionData* a_Action);
	ActionData* getAction(int a_Key);

	void setGravity(float a_Gravity);
	float getGravity();

	void updateAll(float a_Dt);
	void fixedUpdateAll(float a_FixedDt);
	void drawAll();
	void refreshObjects();
	void clear();

	void updatePhysics();
};