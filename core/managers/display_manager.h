#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class DisplayManager : private Manager {
	friend class Engine;
private:
	SDL_Window* m_Window{};
	SDL_Renderer* m_Renderer{};
	Dims m_ScreenDims{};
	Dims m_LogDims{};
	Camera* m_ActiveCamera{};
	bool m_FullscreenEnabled{ false };
	bool m_Borderless{ true };

	DisplayManager(Managers* a_Managers) : Manager(a_Managers) {}
	~DisplayManager() override;
	void destroy() override;

	

	void drawStringOutline(Vector2 a_Pos, const char* a_Text, const Font& a_Font, const Sprite*& a_Sprite, FDims a_MaxSize);
	void drawOutline(const Vector2& a_Pos, const SDL_Rect& a_Src, SDL_FRect& a_Dest, const Outline& a_Outline, SDL_Texture* a_Texture);
	bool checkStringBounds(const FDims& a_MaxSize, const Vector2& a_StartingPos, 
		const Font& a_Font, const Vector2& a_Pos, const Dims& a_CharDims);
public:
	bool init(const char* a_Title, Dims a_WinDims, Dims a_LogDims = {}, bool a_Fullscreen = false);
	void present();
	void clear();

	SDL_Texture* createTexture(SDL_Surface* a_Surface);

	void setIcon(const char* a_FileName, ColorRGBA a_Color = ColorRGBA::black());

	Camera* getActiveCamera() const;
	void setActiveCamera(Camera* a_Camera);

	void showCursor(bool a_Shown);
	bool cursorShown() const;

	Dims getScreenDims() const;
	Dims getLogDims() const;
	int getLogWidth() const;
	int getLogHeight() const;
	Rect getLogRect() const;
	void updateScreenDims();
	Vector2 getLogStart();
	Vector2 getLogScale();

	bool isFullscreen() const;
	void setFullscreen(bool a_Fullscreen);
	void toggleFullscreen();
	Uint32 getFullscreenType() const;
	void setBorderless(bool a_Borderless);

	Vector2 worldToScreen(Vector3 a_WorldPos) const;
	SDL_FRect worldToRect(Vector3 a_WorldPos, FDims a_Dims) const;

	void setDrawColor(ColorRGBA a_Color);
	void setSpriteModColor(const Sprite* a_Sprite, const ColorRGBA& a_Color);
	void drawSprite(int a_SpriteKey, Vector2 a_Pos, FDims a_Dims = {0,0});
	void drawClippedSprite(int a_SpriteKey, Vector2 a_Pos, FDims a_Dims, SDL_Rect a_Clip);
	void drawSprite(int a_SpriteKey, Transform a_Transform);
	void drawAnimFrame(int a_AnimKey, int a_FrameIdx, Transform tr);
	void drawLine(Vector2 a_Start, Vector2 a_Dest, ColorRGBA a_Color);
	void drawRect(Vector2 a_Pos, FDims a_Dims, ColorRGBA a_Color, int a_Thickness = 1);
	void drawFilledRect(Vector2 a_Pos, FDims a_Dims, ColorRGBA a_FillColor, ColorRGBA a_OutlineColor, int a_Thickness = 1);
	void drawString(int a_CharsetKey, Vector2 a_Pos, const char* a_Text, const Font& a_Font, FDims a_MaxSize = { 0,0 });
};