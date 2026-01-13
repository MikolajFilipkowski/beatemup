#include "object_manager.h"

#include "managers.h"
#include "../engine.h"
#include <cstdio>

ObjectManager::~ObjectManager()
{
	destroy();
}

void ObjectManager::destroy()
{
	for (GameObject*& object : m_ObjectArray) {
		delete object;
		object = nullptr;
	}

	for (auto& pair : m_ActionsMap) {
		delete pair.value;
	}

	m_ObjectArray.clear();
	m_ActionsMap.clear();
}

void ObjectManager::startIfNeeded(GameObject*& a_Object)
{
	if (!a_Object->getStartedFlag()) {
		a_Object->start();
		a_Object->setStartedFlag(true);
	}
}

void ObjectManager::add(GameObject* a_Object)
{
	m_ObjectArray.add(a_Object);
}

void ObjectManager::remove(GameObject* a_Object)
{
	a_Object->setRemovalFlag(true);
}

ArrayList<GameObject*>& ObjectManager::getAllObjects()
{
	return m_ObjectArray;
}

Map<int, ActionData*>& ObjectManager::getAllActions()
{
	return m_ActionsMap;
}

void ObjectManager::addAction(int a_Key, ActionData* a_Action)
{
	if (m_ActionsMap.containsKey(a_Key)) return;

	m_ActionsMap.put(a_Key, a_Action);
}

ActionData* ObjectManager::getAction(int a_Key) {
	return m_ActionsMap.get(a_Key);
}

void ObjectManager::setGravity(float a_Gravity)
{
	m_Gravity = a_Gravity;
}

float ObjectManager::getGravity()
{
	return m_Gravity;
}

void ObjectManager::updateAll(float a_Dt)
{
	for (GameObject*& object : m_ObjectArray) {
		if (object != nullptr && !object->getRemovalFlag()) {
			startIfNeeded(object);
			object->update(a_Dt);
		}
	}
}

void ObjectManager::fixedUpdateAll(float a_FixedDt)
{
	for (GameObject*& object : m_ObjectArray) {
		if (object != nullptr && !object->getRemovalFlag()) {
			object->fixedUpdate(a_FixedDt);
			startIfNeeded(object);
		}
	}

	for (GameObject*& object : m_ObjectArray) {
		if (object != nullptr && !object->getRemovalFlag()) {
			object->postFixedUpdate(a_FixedDt);
		}
	}
}

void ObjectManager::drawAll()
{
	m_ObjectArray.sort(
		[](GameObject* const& a_Obj1, GameObject* const& a_Obj2) {
			return a_Obj1->getPosition().z > a_Obj2->getPosition().z;
		}
	);
	for (GameObject*& object : m_ObjectArray) {
		if (object != nullptr)
			object->draw();
	}
}

void ObjectManager::refreshObjects()
{
	m_ObjectArray.removeIf(
		[](GameObject* const& a_Obj) {
			if (a_Obj == nullptr) return false;
			return a_Obj->getRemovalFlag();
		},
		[](GameObject*& a_Obj) {
			delete a_Obj;
			a_Obj = nullptr;
		}
	);
}

void ObjectManager::clear()
{
	destroy();
}

void ObjectManager::updateCombat()
{
}
