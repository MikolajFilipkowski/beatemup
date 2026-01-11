#include "sprite_manager.h"

#include "managers.h"
#include "../engine.h"
#include <stdio.h>

void SpriteManager::load(const char* a_FileName, int a_Key, bool a_IsCharset)
{
	if (m_SpriteMap.containsKey(a_Key))
		return;

	SDL_Surface* surface = SDL_LoadBMP(a_FileName);

	if (surface == NULL) {
		m_Mgs->engine->stop();
		printf("SDL_LoadBMP(%s) error: %s\n", a_FileName, SDL_GetError());
		return;
	}

	if (a_IsCharset)
		SDL_SetColorKey(surface, true, 0x000000);

	int width = surface->w;
	int height = surface->h;

	SDL_Texture* texture = m_Mgs->display->createTexture(surface);

	SDL_FreeSurface(surface);

	if (texture == NULL) {
		m_Mgs->engine->stop();
		printf("SDL_CreateTextureFromSurface(%s) error: %s\n", a_FileName, SDL_GetError());
		return;
	}

	Sprite* sprite = new Sprite();
	sprite->texture = texture;
	sprite->width = width;
	sprite->height = height;

	m_SpriteMap.put(a_Key, sprite);
}

void SpriteManager::loadCharset(const char* a_FileName, int a_Key)
{
	load(a_FileName, a_Key, true);
}

SpriteManager::~SpriteManager()
{
	destroy();
}

void SpriteManager::destroy()
{
	for (auto& pair : m_SpriteMap) {
		if (pair.value == nullptr) continue;

		SDL_DestroyTexture(pair.value->texture);
		delete pair.value;
		pair.value = nullptr;
	}

	m_SpriteMap.clear();
}

void SpriteManager::unload(int a_Key)
{
	if (!m_SpriteMap.containsKey(a_Key))
		return;

	Sprite* sprite = m_SpriteMap.get(a_Key);

	m_SpriteMap.remove(a_Key);

	if (sprite == nullptr) {

		return;
	}

	SDL_DestroyTexture(sprite->texture);

	delete sprite;
	sprite = nullptr;
}

Sprite* SpriteManager::get(int a_Key)
{
	return m_SpriteMap.get(a_Key);
}

void SpriteManager::setSpriteColor(int a_Key, ColorRGBA a_Color)
{
	if (!m_SpriteMap.containsKey(a_Key)) return;

	Sprite* spr = get(a_Key);
	SDL_SetTextureColorMod(spr->texture, a_Color.r, a_Color.g, a_Color.b);
	SDL_SetTextureAlphaMod(spr->texture, a_Color.a);
}

Uint64 SpriteManager::loadedSpritesSize() const
{
	Uint64 size = 0;

	for (auto& spr : m_SpriteMap) {
		int w, h;
		SDL_QueryTexture(spr.value->texture, NULL, NULL, &w, &h);
		size += (Uint64)w * (Uint64)h * 4ULL;
	}

	return size;
}
