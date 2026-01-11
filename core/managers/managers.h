#pragma once

#include "manager.h"
#include "display_manager.h"
#include "sprite_manager.h"
#include "object_manager.h"
#include "time_manager.h"
#include "input_manager.h"
#include "animation_manager.h"
#include "scene_manager.h"
#include "ui_manager.h"

constexpr int MGS_LEN = 8;

class Engine;

class Managers {
	friend class Engine;
private:
	Manager** m_Array[MGS_LEN];
	int m_ArrLen;

	Managers() :
		m_ArrLen(MGS_LEN)
	{
		m_Array[0] = (Manager**)&display;
		m_Array[1] = (Manager**)&sprite;
		m_Array[2] = (Manager**)&object;
		m_Array[3] = (Manager**)&time;
		m_Array[4] = (Manager**)&input;
		m_Array[5] = (Manager**)&anim;
		m_Array[6] = (Manager**)&scene;
		m_Array[7] = (Manager**)&ui;
	}
public:
	DisplayManager* display{};
	SpriteManager* sprite{};
	ObjectManager* object{};
	TimeManager* time{};
	InputManager* input{};
	AnimationManager* anim{};
	SceneManager* scene{};
	UIManager* ui{};

	Engine* engine{};
};