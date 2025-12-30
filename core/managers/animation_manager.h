#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class AnimationManager : private Manager {
	friend class Engine;
private:
	Map<char*> animMap;

	AnimationManager(Managers* managers) : Manager(managers) {}
	~AnimationManager() override;
	void destroy() override;
public:
	
};