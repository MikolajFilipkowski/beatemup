#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class AnimationManager : private Manager {
	friend class Engine;
private:
	Map<int, AnimationClip*> m_AnimMap{};

	AnimationManager(Managers* a_Managers) : Manager(a_Managers) {}
	~AnimationManager() override;
	void destroy() override;
public:
	void createFromSheet(int a_AnimKey, int a_SpriteKey, int a_FrameCount, float a_FrameDuration);
	AnimationClip* get(int a_Key);
	void remove(int a_Key);
};