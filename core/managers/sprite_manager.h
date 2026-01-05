#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class SpriteManager : private Manager {
	friend class Engine;
private:
	Map<int, Sprite*> spriteMap;

	SpriteManager(Managers* managers) : Manager(managers) {}
	~SpriteManager() override;
	void destroy() override;
public:
	void load(const char* fileName, int key, bool isCharset = false);
	void loadCharset(const char* fileName, int key);
	void unload(int key);
	Sprite* get(int key);
	void setSpriteColor(int key, ColorRGBA clr);
};