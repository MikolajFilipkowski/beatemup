#include "display_manager.h"
#include "managers.h"
#include "../engine.h"
#include <stdio.h>

DisplayManager::~DisplayManager()
{
	destroy();
}

Vector2 DisplayManager::worldToScreen(Vector3 worldPos) const
{
	Camera* cam = Camera::getActiveCamera();

	if (cam == nullptr) return { worldPos.x, worldPos.y };

	Vector3 camPos = cam->getPosition();
	float zoom = cam->getZoom();

	Vector2 screenPos{};

	screenPos.x = (worldPos.x - camPos.x) * zoom + (logDims.width / 2.0f);
	screenPos.y = (worldPos.y + worldPos.z - camPos.y) * zoom + (logDims.height / 2.0f);

	return screenPos;
}

bool DisplayManager::init(const char* title, Dims winDims, Dims logicalDims, bool isFullscreen)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return false;
	}

	Uint32 winFlags = (isFullscreen) ? SDL_WINDOW_FULLSCREEN : 0;

	winFlags |= SDL_WINDOW_RESIZABLE;

	sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winDims.width, winDims.height, winFlags);

	if (!sdlWindow) {
		printf("SDL_CreateWindow error: %s\n", SDL_GetError());
		return false;
	}

	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);

	if (!sdlRenderer) {
		SDL_DestroyWindow(sdlWindow);
		printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
		return false;
	}

	if (logicalDims.width == 0 || logicalDims.height == 0) {
		logicalDims.width = winDims.width;
		logicalDims.height = winDims.height;
	}

	screenDims = winDims;

	logDims = logicalDims;

	SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(sdlRenderer, logDims.width, logDims.height);

	return true;
}

void DisplayManager::present()
{
	SDL_RenderPresent(sdlRenderer);
}

void DisplayManager::clear()
{
	SDL_RenderClear(sdlRenderer);
}

void DisplayManager::destroy()
{
	if (sdlRenderer) {
		SDL_DestroyRenderer(sdlRenderer);
		sdlRenderer = nullptr;
	}
	if (sdlWindow) {
		SDL_DestroyWindow(sdlWindow);
		sdlWindow = nullptr;
	}

	SDL_Quit();
}

SDL_Renderer* DisplayManager::getSDL_Renderer()
{
	return sdlRenderer;
}

SDL_Texture* DisplayManager::createTexture(SDL_Surface* surface)
{
	return SDL_CreateTextureFromSurface(sdlRenderer, surface);
}

Dims DisplayManager::getScreenDims() const
{
	return screenDims;
}

Dims DisplayManager::getLogDims() const
{
	return logDims;
}

int DisplayManager::getLogWidth() const
{
	return logDims.width;
}

int DisplayManager::getLogHeight() const
{
	return logDims.height;
}

void DisplayManager::updateScreenDims()
{
	SDL_GetWindowSize(sdlWindow, &screenDims.width, &screenDims.height);
}

Vector2 DisplayManager::getLogStart()
{
	SDL_Rect viewport;
	SDL_RenderGetViewport(sdlRenderer, &viewport);
	return { 
		(float)viewport.x, 
		(float)viewport.y 
	};
}

Vector2 DisplayManager::getLogScale()
{
	float scaleX, scaleY;
	SDL_RenderGetScale(sdlRenderer, &scaleX, &scaleY);

	return {
		scaleX,
		scaleY
	};
}

void DisplayManager::setDrawColor(ColorRGBA color)
{
	SDL_SetRenderDrawColor(sdlRenderer, color.r, color.g, color.b, color.a);
}

void DisplayManager::drawSprite(const char* sprite_key, Vector2 pos)
{
	Sprite* sprite = mgs->sprite->get(sprite_key);
	if (sprite == nullptr || sprite->texture == nullptr) return;

	SDL_FRect rect = { pos.x, pos.y, (float)sprite->width, (float)sprite->height };

	SDL_RenderCopyF(sdlRenderer, sprite->texture, nullptr, &rect);
}

void DisplayManager::drawSprite(const char* sprite_key, Vector3 worldPos)
{
	Sprite* sprite = mgs->sprite->get(sprite_key);
	if (sprite == nullptr || sprite->texture == nullptr) return;

	Vector2 screenPos = worldToScreen(worldPos);
	float zoom = Camera::getActiveCamera()->getZoom();

	SDL_FRect rect{};

	rect.w = sprite->width * zoom;
	rect.h = sprite->height * zoom;

	// (0,0) -> x srodek, y dol
	rect.x = screenPos.x - (rect.w / 2.0f);
	rect.y = screenPos.y - rect.h;

	SDL_RenderCopyF(sdlRenderer, sprite->texture, nullptr, &rect);
}

void DisplayManager::drawLine(Vector2 start, Vector2 dest, ColorRGBA color)
{
	setDrawColor(color);

	SDL_RenderDrawLineF(sdlRenderer, start.x, start.y, dest.x, dest.y);

	setDrawColor(ColorRGBA::black());
}

void DisplayManager::drawRect(Vector2 pos, Dims dims, ColorRGBA color, int thickness)
{
	setDrawColor(color);

	for (int i = 0; i < thickness; i++) {
		SDL_FRect rect = { 
			pos.x + i,
			pos.y + i,
			(float)(dims.width - i * 2),
			(float)(dims.height - i * 2)
		};
		SDL_RenderDrawRectF(sdlRenderer, &rect);
	}

	setDrawColor(ColorRGBA::black());
}

void DisplayManager::drawFilledRect(Vector2 pos, Dims dims, ColorRGBA fill_color, ColorRGBA outline_color, int thickness)
{
	setDrawColor(fill_color);

	SDL_FRect fill_rect = { 
		pos.x,
		pos.y,
		(float)dims.width,
		(float)dims.height
	};

	SDL_RenderFillRectF(sdlRenderer, &fill_rect);

	drawRect(pos, dims, outline_color, thickness);
}

void DisplayManager::drawString(const char* charset_key, Vector2 pos, const char* text)
{
	Sprite* sprite = mgs->sprite->get(charset_key);

	int px, py, c;
	SDL_Rect s;
	SDL_FRect d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = pos.x;
		d.y = pos.y;

		SDL_RenderCopyF(sdlRenderer, sprite->texture, &s, &d);

		pos.x += 8;
		text++;
	};
}