#pragma once

#include "manager.h"
#include "display_manager.h"
#include "sprite_manager.h"
#include "object_manager.h"
#include "time_manager.h"
#include "input_manager.h"
#include "animation_manager.h"

constexpr int MGS_LEN = 6;

class Engine;

class Managers {
	friend class Engine;
private:
	Manager** array[MGS_LEN];
	int arrLen;
public:
	DisplayManager* display = nullptr;
	SpriteManager* sprite = nullptr;
	ObjectManager* object = nullptr;
	TimeManager* time = nullptr;
	InputManager* input = nullptr;
	AnimationManager* anim = nullptr;

	Engine* engine = nullptr;

	Managers() :
		arrLen(MGS_LEN)
	{
		array[0] = (Manager**)&display;
		array[1] = (Manager**)&sprite;
		array[2] = (Manager**)&object;
		array[3] = (Manager**)&time;
		array[4] = (Manager**)&input;
		array[5] = (Manager**)&anim;
	}
};