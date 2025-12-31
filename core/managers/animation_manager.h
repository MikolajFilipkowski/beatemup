#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class AnimationManager : private Manager {
	friend class Engine;
private:
	Map<AnimationClip*> animMap;

	AnimationManager(Managers* managers) : Manager(managers) {}
	~AnimationManager() override;
	void destroy() override;
public:
	void createFromSheet(const char* animKey, const char* spriteKey, int frameCount, float frameDuration);
	AnimationClip*& get(const char* key);
	void remove(const char* key);
};