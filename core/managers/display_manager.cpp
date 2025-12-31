#include "display_manager.h"
#include "managers.h"
#include "../engine.h"
#include <stdio.h>
#include <assert.h>

DisplayManager::~DisplayManager()
{
	destroy();
}

Vector2 DisplayManager::worldToScreen(Vector3 worldPos) const
{
	Camera* cam = getActiveCamera();

	if (cam == nullptr) return { worldPos.x, worldPos.y };

	Vector3 camPos = cam->getPosition();
	float zoom = cam->getZoom();

	Vector2 screenPos{};

	screenPos.x = (worldPos.x - camPos.x) * zoom + (logDims.width / 2.0f);
	screenPos.y = (worldPos.y + worldPos.z - camPos.y) * zoom + (logDims.height / 2.0f);

	return screenPos;
}

SDL_FRect DisplayManager::worldToRect(Vector3 worldPos, Dims dims) const
{
	Vector2 screenPos = worldToScreen(worldPos);
	float zoom = getActiveCamera()->getZoom();

	SDL_FRect rect{};

	rect.w = dims.width * zoom;
	rect.h = dims.height * zoom;

	// (0,0) -> x srodek, y dol
	rect.x = screenPos.x - (rect.w / 2.0f);
	rect.y = screenPos.y - rect.h;

	printf("x: %f y: %f w: %f h: %f\n", rect.x, rect.y, rect.w, rect.h);

	return rect;
}

bool DisplayManager::init(const char* title, Dims winDims, Dims logicalDims, bool fullscreen)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return false;
	}

	Uint32 winFlags = (fullscreen) ? SDL_WINDOW_FULLSCREEN : 0;
	fullscreenEnabled = fullscreen;

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
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
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

Camera* DisplayManager::getActiveCamera() const
{
	return activeCamera;
}

void DisplayManager::setActiveCamera(Camera* camera)
{
	activeCamera = camera;
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

bool DisplayManager::isFullscreen() const
{
	return fullscreenEnabled;
}

void DisplayManager::setFullscreen(bool flag)
{
	fullscreenEnabled = flag;
	SDL_SetWindowFullscreen(sdlWindow, getFullscreenType());
}

void DisplayManager::toggleFullscreen()
{
	fullscreenEnabled = !fullscreenEnabled;
	SDL_SetWindowFullscreen(sdlWindow, getFullscreenType());
}

Uint32 DisplayManager::getFullscreenType() const
{
	if (!fullscreenEnabled)
		return 0;
	return (borderlessFullscreen) ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_FULLSCREEN;
}

void DisplayManager::setBorderless(bool isBorderlessed)
{
	borderlessFullscreen = isBorderlessed;
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

	SDL_FRect rect = worldToRect(worldPos, { sprite->width, sprite->height });

	SDL_RenderCopyF(sdlRenderer, sprite->texture, nullptr, &rect);
}

void DisplayManager::drawAnimFrame(const char* anim_key, int frameIdx, 
	Vector3 worldPos, SDL_RendererFlip flip, double rotation
) {
	AnimationClip* clip = mgs->anim->get(anim_key);
	if (clip == nullptr) return;

	if (frameIdx >= clip->frameCount) {
		assert(false && "Frame out of bounds");
		return;
	}

	Sprite* sprite = mgs->sprite->get(clip->spriteKey);
	if (sprite == nullptr) return;

	SDL_Rect src = clip->frames[frameIdx];

	SDL_FRect dst = worldToRect(worldPos, { src.w, src.h });

	SDL_RenderCopyExF(sdlRenderer, sprite->texture, &src, &dst, rotation, NULL, flip);
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

void DisplayManager::drawString(const char* charset_key, Vector2 pos, const char* text, float scale)
{
	Sprite* sprite = mgs->sprite->get(charset_key);
	if (sprite == nullptr) return;

	int charW = sprite->width / 16;
	int charH = sprite->height / 16;

	int px, py, ch;
	SDL_Rect src;
	SDL_FRect dest;
	src.w = charW;
	src.h = charH;

	dest.w = charW * scale;
	dest.h = charH * scale;

	while (*text) {
		ch = *text & 255;
		px = (ch % 16) * charW;
		py = (ch / 16) * charH;
		src.x = px;
		src.y = py;
		dest.x = pos.x * scale;
		dest.y = pos.y * scale;

		SDL_RenderCopyF(sdlRenderer, sprite->texture, &src, &dest);

		pos.x += charW;
		text++;
	};
}