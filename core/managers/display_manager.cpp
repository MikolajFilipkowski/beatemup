#include "display_manager.h"
#include "managers.h"
#include "../engine.h"
#include <stdio.h>
#include <assert.h>

DisplayManager::~DisplayManager()
{
	destroy();
}

void DisplayManager::drawStringOutline(Vector2 a_Pos, const char* a_Text, const Font& a_Font, const Sprite*& a_Sprite, FDims a_MaxSize)
{
	Dims charDims = { a_Sprite->width / 16 , a_Sprite->height / 16 };

	const ColorRGBA& clr = a_Font.outline.color;

	int ch;
	SDL_Rect src = { 0, 0, charDims.width, charDims.height };
	SDL_FRect dest = {
		0, 0,
		charDims.width * a_Font.scale,
		charDims.height * a_Font.scale
	};

	Vector2 startingPos = a_Pos;

	float advX = dest.w * a_Font.spacing;

	setSpriteModColor(a_Sprite, clr);

	while (*a_Text) {
		ch = *a_Text & 255;
		src.x = (ch % 16) * charDims.width;
		src.y = (ch / 16) * charDims.height;

		if (!checkStringBounds(a_MaxSize, startingPos, a_Font, a_Pos, charDims)) break;

		dest.x = a_Pos.x;
		dest.y = a_Pos.y;

		drawOutline(a_Pos, src, dest, a_Font.outline, a_Sprite->texture);

		a_Pos.x += advX;
		a_Text++;
	};
}

void DisplayManager::drawOutline(const Vector2& a_Pos, const SDL_Rect& a_Src, SDL_FRect& a_Dest, const Outline& a_Outline, SDL_Texture* a_Texture)
{
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			if (x == 0 && y == 0) continue;

			a_Dest.x = a_Pos.x + x * a_Outline.size;
			a_Dest.y = a_Pos.y + y * a_Outline.size;
			SDL_RenderCopyF(m_Renderer, a_Texture, &a_Src, &a_Dest);
		}
	}
}

bool DisplayManager::checkStringBounds(const FDims& a_MaxSize, const Vector2& a_StartingPos, 
	const Font& a_Font, const Vector2& a_Pos, const Dims& a_CharDims)
{
	const float w = a_CharDims.width * a_Font.spacing + a_Pos.x;
	const float h = a_CharDims.height * a_Font.spacing + a_Pos.y;
	if (a_MaxSize.width != 0 && a_MaxSize.height != 0) {
		if ((w > a_MaxSize.width + a_StartingPos.x) || 
			(h > a_MaxSize.height + a_StartingPos.y)
			) {
			return false;
		}
	}
	return true;
}

bool DisplayManager::init(const char* a_Title, Dims a_WinDims, Dims a_LogDims, bool a_Fullscreen)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return false;
	}

	Uint32 winFlags = (a_Fullscreen) ? SDL_WINDOW_FULLSCREEN : 0;
	m_FullscreenEnabled = a_Fullscreen;

	winFlags |= SDL_WINDOW_RESIZABLE;

	m_Window = SDL_CreateWindow(a_Title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, a_WinDims.width, a_WinDims.height, winFlags);

	if (!m_Window) {
		printf("SDL_CreateWindow error: %s\n", SDL_GetError());
		return false;
	}

	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

	if (!m_Renderer) {
		SDL_DestroyWindow(m_Window);
		printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
		return false;
	}

	if (a_LogDims.width == 0 || a_LogDims.height == 0) {
		a_LogDims.width = a_WinDims.width;
		a_LogDims.height = a_WinDims.height;
	}

	m_ScreenDims = a_WinDims;

	m_LogDims = a_LogDims;
	m_LogRect = { 0.0f,0.0f, (float)m_LogDims.width, (float)m_LogDims.height };

	SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	SDL_RenderSetLogicalSize(m_Renderer, m_LogDims.width, m_LogDims.height);

	return true;
}

void DisplayManager::present()
{
	SDL_RenderPresent(m_Renderer);
}

void DisplayManager::clear()
{
	SDL_RenderClear(m_Renderer);
}

void DisplayManager::destroy()
{
	if (m_Renderer) {
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
	if (m_Window) {
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
	}

	SDL_Quit();
}

SDL_Texture* DisplayManager::createTexture(SDL_Surface* a_Surface)
{
	return SDL_CreateTextureFromSurface(m_Renderer, a_Surface);
}

void DisplayManager::setIcon(const char* a_FileName, ColorRGBA a_Color)
{
	SDL_Surface* icon = SDL_LoadBMP(a_FileName);
	if (icon == nullptr) return;

	SDL_LockSurface(icon);

	Uint32* pixels = (Uint32*)icon->pixels;
	int pixelC = icon->w * icon->h;

	for (int i = 0; i < pixelC; i++) {
		Uint8 dummy, a;
		SDL_GetRGBA(pixels[i], icon->format, &dummy, &dummy, &dummy, &a);

		if (a != 0x00) {
			pixels[i] = SDL_MapRGBA(icon->format, a_Color.r, a_Color.g, a_Color.b, a_Color.a);
		}
	}

	SDL_UnlockSurface(icon);

	SDL_SetWindowIcon(m_Window, icon);
	SDL_FreeSurface(icon);
}

Camera* DisplayManager::getActiveCamera() const
{
	return m_ActiveCamera;
}

void DisplayManager::setActiveCamera(Camera* a_Camera)
{
	m_ActiveCamera = a_Camera;
}

void DisplayManager::showCursor(bool shown)
{
	SDL_ShowCursor((int)shown);
}

bool DisplayManager::cursorShown() const
{
	return (bool)SDL_ShowCursor(-1);
}

const Dims& DisplayManager::getScreenDims() const
{
	return m_ScreenDims;
}

const Dims& DisplayManager::getLogDims() const
{
	return m_LogDims;
}

int DisplayManager::getLogWidth() const
{
	return m_LogDims.width;
}

int DisplayManager::getLogHeight() const
{
	return m_LogDims.height;
}

const Rect& DisplayManager::getLogRect() const
{
	return m_LogRect;
}

void DisplayManager::updateScreenDims()
{
	SDL_GetWindowSize(m_Window, &m_ScreenDims.width, &m_ScreenDims.height);
}

Vector2 DisplayManager::getLogStart()
{
	SDL_Rect viewport;
	SDL_RenderGetViewport(m_Renderer, &viewport);
	return { 
		(float)viewport.x, 
		(float)viewport.y 
	};
}

Vector2 DisplayManager::getLogScale()
{
	float scaleX, scaleY;
	SDL_RenderGetScale(m_Renderer, &scaleX, &scaleY);

	return {
		scaleX,
		scaleY
	};
}

bool DisplayManager::isFullscreen() const
{
	return m_FullscreenEnabled;
}

void DisplayManager::setFullscreen(bool a_Flag)
{
	m_FullscreenEnabled = a_Flag;
	SDL_SetWindowFullscreen(m_Window, getFullscreenType());
}

void DisplayManager::toggleFullscreen()
{
	m_FullscreenEnabled = !m_FullscreenEnabled;
	SDL_SetWindowFullscreen(m_Window, getFullscreenType());
}

Uint32 DisplayManager::getFullscreenType() const
{
	if (!m_FullscreenEnabled)
		return 0;
	return (m_Borderless) ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_FULLSCREEN;
}

void DisplayManager::setBorderless(bool a_Borderless)
{
	m_Borderless = a_Borderless;
}

Vector2 DisplayManager::worldToScreen(Vector3 a_WorldPos) const
{
	Camera* cam = getActiveCamera();

	if (cam == nullptr) return { a_WorldPos.x, a_WorldPos.y };

	Vector3 camPos = cam->getPosition();
	float zoom = cam->getZoom();

	Vector2 screenPos{};

	screenPos.x = (a_WorldPos.x - camPos.x) * zoom + (m_LogDims.width / 2.0f);
	screenPos.y = (a_WorldPos.y + a_WorldPos.z - camPos.y) * zoom + (m_LogDims.height / 2.0f);

	return screenPos;
}

SDL_FRect DisplayManager::worldToRect(Vector3 a_WorldPos, FDims a_Dims) const
{
	Vector2 screenPos = worldToScreen(a_WorldPos);
	float zoom = getActiveCamera()->getZoom();

	SDL_FRect rect{};

	rect.w = a_Dims.width * zoom;
	rect.h = a_Dims.height * zoom;

	// (0,0) -> x srodek, y dol
	rect.x = screenPos.x - (rect.w / 2.0f);
	rect.y = screenPos.y - rect.h;

	//printf("x: %f y: %f w: %f h: %f\n", rect.x, rect.y, rect.w, rect.h);

	return rect;
}

void DisplayManager::setDrawColor(ColorRGBA a_Color)
{
	SDL_SetRenderDrawColor(m_Renderer, a_Color.r, a_Color.g, a_Color.b, a_Color.a);
}

void DisplayManager::setSpriteModColor(const Sprite* a_Sprite, const ColorRGBA& a_Color)
{
	SDL_SetTextureColorMod(a_Sprite->texture, a_Color.r, a_Color.g, a_Color.b);
	SDL_SetTextureAlphaMod(a_Sprite->texture, a_Color.a);
}

void DisplayManager::drawSprite(int a_SpriteKey, Vector2 a_Pos, FDims a_Dims)
{
	Sprite* sprite = m_Mgs->sprite->get(a_SpriteKey);
	if (sprite == nullptr || sprite->texture == nullptr) return;

	if (a_Dims.width == 0 || a_Dims.height == 0) {
		a_Dims.width = (float)sprite->width;
		a_Dims.height = (float)sprite->height;
	}

	SDL_FRect rect = { a_Pos.x, a_Pos.y, a_Dims.width, a_Dims.height };

	SDL_RenderCopyF(m_Renderer, sprite->texture, nullptr, &rect);
}

void DisplayManager::drawClippedSprite(int a_SpriteKey, Vector2 a_Pos, FDims a_Dims, SDL_Rect a_Clip)
{
	Sprite* sprite = m_Mgs->sprite->get(a_SpriteKey);
	if (sprite == nullptr || sprite->texture == nullptr) return;

	if (a_Dims.width == 0 || a_Dims.height == 0) {
		a_Dims.width = (float)sprite->width;
		a_Dims.height = (float)sprite->height;
	}

	SDL_FRect rect = { a_Pos.x, a_Pos.y, a_Dims.width, a_Dims.height };
	if (!getLogRect().intersects(rect)) return;

	SDL_RenderCopyF(m_Renderer, sprite->texture, &a_Clip, &rect);
}

void DisplayManager::drawSprite(int a_SpriteKey, Transform a_Transform)
{
	Sprite* sprite = m_Mgs->sprite->get(a_SpriteKey);
	if (sprite == nullptr || sprite->texture == nullptr) return;

	SDL_FRect rect = worldToRect(a_Transform.pos, { sprite->width * a_Transform.scale.x, sprite->height * a_Transform.scale.y });
	if (!getLogRect().intersects(rect)) return;

	SDL_RenderCopyExF(m_Renderer, sprite->texture, nullptr, &rect, a_Transform.rotation, NULL, a_Transform.flip);
}

void DisplayManager::drawAnimFrame(int a_AnimKey, int a_FrameIdx, Transform a_Transform) {
	AnimationClip* clip = m_Mgs->anim->get(a_AnimKey);
	if (clip == nullptr) return;

	if (a_FrameIdx >= clip->frameCount) {
		assert(false && "Frame out of bounds");
		return;
	}

	Sprite* sprite = m_Mgs->sprite->get(clip->spriteKey);
	if (sprite == nullptr) return;

	SDL_Rect src = clip->frames[a_FrameIdx];

	SDL_FRect dst = worldToRect(a_Transform.pos, { src.w * a_Transform.scale.x, src.h * a_Transform.scale.y });
	if (!getLogRect().intersects(dst)) return;

	SDL_RenderCopyExF(m_Renderer, sprite->texture, &src, &dst, a_Transform.rotation, NULL, a_Transform.flip);
}

void DisplayManager::drawLine(Vector2 a_Start, Vector2 a_Dest, ColorRGBA a_Color)
{
	setDrawColor(a_Color);

	SDL_RenderDrawLineF(m_Renderer, a_Start.x, a_Start.y, a_Dest.x, a_Dest.y);

	setDrawColor(ColorRGBA::black());
}

void DisplayManager::drawRect(Vector2 a_Pos, FDims a_Dims, ColorRGBA a_Color, int a_Thickness)
{
	setDrawColor(a_Color);
	
	for (int i = 0; i < a_Thickness; i++) {
		SDL_FRect rect = { 
			a_Pos.x + i,
			a_Pos.y + i,
			(float)(a_Dims.width - i * 2),
			(float)(a_Dims.height - i * 2)
		};
		if (!getLogRect().intersects(rect)) continue;
		SDL_RenderDrawRectF(m_Renderer, &rect);
	}

	setDrawColor(ColorRGBA::black());
}

void DisplayManager::drawFilledRect(Vector2 a_Pos, FDims a_Dims, ColorRGBA a_FillColor, ColorRGBA a_OutlineColor, int a_Thickness)
{
	setDrawColor(a_FillColor);

	SDL_FRect fill_rect = { 
		a_Pos.x,
		a_Pos.y,
		a_Dims.width,
		a_Dims.height
	};

	SDL_RenderFillRectF(m_Renderer, &fill_rect);

	drawRect(a_Pos, a_Dims, a_OutlineColor, a_Thickness);
}

void DisplayManager::drawString(int a_CharsetKey, Vector2 a_Pos, const char* a_Text, const Font& a_Font, FDims a_MaxSize)
{
	if (a_Text == nullptr || *a_Text == '\0') return;
	const Sprite* sprite = m_Mgs->sprite->get(a_CharsetKey);
	if (sprite == nullptr) return;

	Dims charDims = { sprite->width / 16 , sprite->height / 16 };

	const ColorRGBA& clr = a_Font.color;

	int ch;
	SDL_Rect src = { 0, 0, charDims.width, charDims.height };
	SDL_FRect dest = { 
		0, 0, 
		charDims.width * a_Font.scale, 
		charDims.height * a_Font.scale 
	};

	a_Pos -= {0, a_Font.baseline * a_Font.scale};
	Vector2 starting_pos = a_Pos;

	float advX = dest.w * a_Font.spacing;

	if (a_Font.outline.size > 0)
		drawStringOutline(a_Pos, a_Text, a_Font, sprite, a_MaxSize);

	setSpriteModColor(sprite, clr);

	while (*a_Text) {
		ch = *a_Text & 255;
		src.x = (ch % 16) * charDims.width;
		src.y = (ch / 16) * charDims.height;

		if (!checkStringBounds(a_MaxSize, starting_pos, a_Font, a_Pos, charDims)) break;

		dest.x = a_Pos.x;
		dest.y = a_Pos.y;

		SDL_RenderCopyF(m_Renderer, sprite->texture, &src, &dest);

		a_Pos.x += advX;
		a_Text++;
	};

	setSpriteModColor(sprite, ColorRGBA::white());
}