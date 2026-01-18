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
	clear();

	for (auto& pair : m_ActionsMap) {
		delete pair.value;
	}
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

	updatePhysics();

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
	for (GameObject*& object : m_ObjectArray) {
		delete object;
		object = nullptr;
	}
	m_ObjectArray.clear();
}

void ObjectManager::updatePhysics()
{
	for (int i = 0; i < m_ObjectArray.count(); i++) {
		GameObject*& obj1 = m_ObjectArray[i];
		Rigidbody& rb1 = obj1->getRb();
		Vector3& pos = rb1.currPos;
		if (obj1->getRemovalFlag()) continue;

		if (rb1.bounds.applyBounds) {
			if (pos.x < rb1.bounds.minX) pos.x = rb1.bounds.minX;
			if (pos.x > rb1.bounds.maxX) pos.x = rb1.bounds.maxX;
			if (pos.z < rb1.bounds.minZ) pos.z = rb1.bounds.minZ;
			if (pos.z > rb1.bounds.maxZ) pos.z = rb1.bounds.maxZ;
		}

		Cuboid coll1 = obj1->getCollBox();
		if (coll1.h == 0 || coll1.w == 0 || coll1.d == 0)
			continue;
		
		for (int j = i + 1; j < m_ObjectArray.count(); j++) {
			GameObject*& obj2 = m_ObjectArray[j];
			Rigidbody& rb2 = obj2->getRb();

			if (obj2 == obj1 || obj2->getRemovalFlag()) 
				continue;
			Cuboid coll2 = obj2->getCollBox();

			Vector3 ov = coll1.overlap(coll2);
			if (ov.x == 0 || ov.y == 0 || ov.z == 0)
				continue;

			
			float weight1, weight2;
			if (rb1.mass <= 0 && rb2.mass <= 0) 
				continue;
			else if (rb1.mass <= 0) {
				weight1 = 0.0f;
				weight2 = 1.0f;
			}
			else if (rb2.mass <= 0) {
				weight2 = 0.0f;
				weight1 = 1.0f;
			}
			else {
				float totalMass = rb1.mass + rb2.mass;
				if (totalMass <= 0) continue;
				weight1 = rb2.mass / totalMass;
				weight2 = rb1.mass / totalMass;
			}
				

			float dir;
			if (ov.x < ov.z) {
				dir = (coll1.x > coll2.x) ? 1.0f : -1.0f;
				rb1.currPos.x += ov.x * dir * weight1;
				rb2.currPos.x -= ov.x * dir * weight2;
			}
			else {
				dir = (coll1.z > coll2.z) ? 1.0f : -1.0f;
				rb1.currPos.z += ov.z * dir * weight1;
				rb2.currPos.z -= ov.z * dir * weight2;
			}

			coll1 = obj1->getCollBox();
		}
	}
}
