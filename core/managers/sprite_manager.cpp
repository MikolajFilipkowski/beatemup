#include "sprite_manager.h"

#include "managers.h"
#include "../engine.h"
#include <stdio.h>

void SpriteManager::load(const char* fileName, int key, bool isCharset)
{
	if (spriteMap.containsKey(key))
		return;

	SDL_Surface* surface = SDL_LoadBMP(fileName);

	if (surface == NULL) {
		mgs->engine->stop();
		printf("SDL_LoadBMP(%s) error: %s\n", fileName, SDL_GetError());
		return;
	}

	if (isCharset)
		SDL_SetColorKey(surface, true, 0x000000);

	int width = surface->w;
	int height = surface->h;

	SDL_Texture* texture = mgs->display->createTexture(surface);

	SDL_FreeSurface(surface);

	if (texture == NULL) {
		mgs->engine->stop();
		printf("SDL_CreateTextureFromSurface(%s) error: %s\n", fileName, SDL_GetError());
		return;
	}

	Sprite* sprite = new Sprite();
	sprite->texture = texture;
	sprite->width = width;
	sprite->height = height;

	spriteMap.put(key, sprite);
}

void SpriteManager::loadCharset(const char* fileName, int key)
{
	load(fileName, key, true);
}

SpriteManager::~SpriteManager()
{
	destroy();
}

void SpriteManager::destroy()
{
	for (auto& pair : spriteMap) {
		if (pair.value == nullptr) continue;

		SDL_DestroyTexture(pair.value->texture);
		delete pair.value;
		pair.value = nullptr;
	}

	spriteMap.clear();
}

void SpriteManager::unload(int key)
{
	if (!spriteMap.containsKey(key))
		return;

	Sprite* sprite = spriteMap.get(key);

	spriteMap.remove(key);

	if (sprite == nullptr) {

		return;
	}

	SDL_DestroyTexture(sprite->texture);

	delete sprite;
	sprite = nullptr;
}

Sprite* SpriteManager::get(int key)
{
	return spriteMap.get(key);
}

void SpriteManager::setSpriteColor(int key, ColorRGBA clr)
{
	if (!spriteMap.containsKey(key)) return;

	Sprite* spr = get(key);
	SDL_SetTextureColorMod(spr->texture, clr.r, clr.g, clr.b);
	SDL_SetTextureAlphaMod(spr->texture, clr.a);
}

Uint64 SpriteManager::loadedSpritesSize() const
{
	Uint64 size = 0;

	for (auto& spr : spriteMap) {
		int w, h;
		SDL_QueryTexture(spr.value->texture, NULL, NULL, &w, &h);
		size += (Uint64)w * (Uint64)h * 4ULL;
	}

	return size;
}
