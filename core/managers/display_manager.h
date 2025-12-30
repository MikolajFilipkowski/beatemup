#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class DisplayManager : private Manager {
	friend class Engine;
private:
	SDL_Window* sdlWindow;
	SDL_Renderer* sdlRenderer;
	Dims screenDims;
	Dims logDims;

	DisplayManager(Managers* managers) : Manager(managers), sdlWindow(nullptr), sdlRenderer(nullptr), screenDims{}, logDims{} {}
	~DisplayManager() override;
	void destroy() override;

	Vector2 worldToScreen(Vector3 worldPos) const;
public:
	bool init(const char* title, Dims winDims, Dims logicalDims = {}, bool isFullscreen = false);
	void present();
	void clear();

	SDL_Renderer* getSDL_Renderer();
	SDL_Texture* createTexture(SDL_Surface* surface);

	Dims getScreenDims() const;
	Dims getLogDims() const;
	int getLogWidth() const;
	int getLogHeight() const;
	void updateScreenDims();
	Vector2 getLogStart();
	Vector2 getLogScale();

	void setDrawColor(ColorRGBA color);
	void drawSprite(const char* sprite_key, Vector2 pos);
	void drawSprite(const char* sprite_key, Vector3 worldPos);
	void drawLine(Vector2 start, Vector2 dest, ColorRGBA color);
	void drawRect(Vector2 pos, Dims dims, ColorRGBA color, int thickness = 1);
	void drawFilledRect(Vector2 pos, Dims dims, ColorRGBA fill_color, ColorRGBA outline_color, int thickness = 1);
	void drawString(const char* charset_key, Vector2 pos, const char* text);
};