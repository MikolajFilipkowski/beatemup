#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class SpriteManager : private Manager {
	friend class Engine;
private:
	Map<int, Sprite*> m_SpriteMap{};

	SpriteManager(Managers* a_Managers) : Manager(a_Managers) {}
	~SpriteManager() override;
	void destroy() override;
public:
	void load(const char* a_FileName, int a_Key, bool a_IsCharset = false);
	void loadCharset(const char* a_FileName, int a_Key);
	void unload(int a_Key);
	Sprite* get(int a_Key);
	void setSpriteColor(int a_Key, ColorRGBA a_Color);
	Uint64 loadedSpritesSize() const;
};