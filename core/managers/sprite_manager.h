#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class SpriteManager : private Manager {
	friend class Engine;
private:
	Map<Sprite*> spriteMap;

	SpriteManager(Managers* managers) : Manager(managers) {}
	~SpriteManager() override;
	void destroy() override;
public:
	void load(const char* fileName, const char* key, bool isCharset = false);
	void loadCharset(const char* fileName, const char* key);
	void unload(const char* key);
	Sprite* get(const char* key);
};