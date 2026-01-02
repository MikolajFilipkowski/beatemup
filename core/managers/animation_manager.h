#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class AnimationManager : private Manager {
	friend class Engine;
private:
	Map<int, AnimationClip*> animMap;

	AnimationManager(Managers* managers) : Manager(managers) {}
	~AnimationManager() override;
	void destroy() override;
public:
	void createFromSheet(int animKey, int spriteKey, int frameCount, float frameDuration);
	AnimationClip* get(int key);
	void remove(int key);
};