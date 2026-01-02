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
	Camera* activeCamera;
	bool fullscreenEnabled;
	bool borderlessFullscreen;

	DisplayManager(Managers* managers) : Manager(managers), sdlWindow(nullptr), sdlRenderer(nullptr),
		screenDims{}, logDims{}, activeCamera(nullptr), fullscreenEnabled(false), borderlessFullscreen(true) {}
	~DisplayManager() override;
	void destroy() override;

	Vector2 worldToScreen(Vector3 worldPos) const;
	SDL_FRect worldToRect(Vector3 worldPos, FDims dims) const;
public:
	bool init(const char* title, Dims winDims, Dims logicalDims = {}, bool fullscreen = false);
	void present();
	void clear();

	SDL_Renderer* getSDL_Renderer();
	SDL_Texture* createTexture(SDL_Surface* surface);

	Camera* getActiveCamera() const;
	void setActiveCamera(Camera* camera);

	Dims getScreenDims() const;
	Dims getLogDims() const;
	int getLogWidth() const;
	int getLogHeight() const;
	void updateScreenDims();
	Vector2 getLogStart();
	Vector2 getLogScale();

	bool isFullscreen() const;
	void setFullscreen(bool isFullscreened);
	void toggleFullscreen();
	Uint32 getFullscreenType() const;
	void setBorderless(bool isBorderlessed);

	void setDrawColor(ColorRGBA color);
	void drawSprite(int sprite_key, Vector2 pos);
	void drawSprite(int sprite_key, Transform tr);
	void drawAnimFrame(int anim_key, int frameIdx, Transform tr);
	void drawLine(Vector2 start, Vector2 dest, ColorRGBA color);
	void drawRect(Vector2 pos, Dims dims, ColorRGBA color, int thickness = 1);
	void drawFilledRect(Vector2 pos, Dims dims, ColorRGBA fill_color, ColorRGBA outline_color, int thickness = 1);
	void drawString(int charset_key, Vector2 pos, const char* text, float scale = 1.0f, float spacing = 1.0f);
};