#include "object_manager.h"

#include "managers.h"

void ObjectManager::add(GameObject* object)
{
	objectArray.add(object);
}

void ObjectManager::remove(GameObject* object)
{
	object->setRemovalFlag(true);
}

void ObjectManager::updateAll(float dt)
{
	for (GameObject*& object : objectArray) {
		if (!object->getRemovalFlag())
			object->update(dt);
	}
}

void ObjectManager::fixedUpdateAll(float fixed_dt)
{
	for (GameObject*& object : objectArray) {
		if (!object->getRemovalFlag())
			object->fixedUpdate(fixed_dt);
	}
}

void ObjectManager::drawAll()
{
	objectArray.sort(
		[](GameObject* const& obj1, GameObject* const& obj2) {
			return obj1->getPosition().z > obj2->getPosition().z;
		}
	);
	for (GameObject*& object : objectArray) {
		object->draw();
	}
}

void ObjectManager::refreshObjects()
{
	objectArray.removeIf(
		[](GameObject* const& obj) {
			return obj->getRemovalFlag();
		},
		[](GameObject*& obj) {
			delete obj;
			obj = nullptr;
		}
	);
}

ObjectManager::~ObjectManager()
{
	destroy();
}

void ObjectManager::destroy()
{
	for (GameObject*& object : objectArray) {
		delete object;
		object = nullptr;
	}

	objectArray.clear();
}