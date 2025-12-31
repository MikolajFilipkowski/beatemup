#pragma once

#include "utils.h"

extern "C" {
#include"./SDL2/include/SDL.h"
}

constexpr float DEFAULT_FOV = 90.0f;
constexpr float DEFAULT_CAM_Z = 500.0f;
constexpr float FIXED_DT = 0.01f;
constexpr float MAX_DT = 0.25f;
constexpr float FPS_CAP = 240.0f;
constexpr Uint32 MIN_FRAMETIME = (Uint32)(1000 / FPS_CAP);
constexpr Uint32 MAX_FRAMETIME = 500;
constexpr float FPS_INTERVAL = 1.0f;

class Managers;

typedef SDL_RendererFlip Flip;

constexpr SDL_RendererFlip NO_FLIP = SDL_FLIP_NONE;
constexpr SDL_RendererFlip H_FLIP = SDL_FLIP_HORIZONTAL;
constexpr SDL_RendererFlip V_FLIP = SDL_FLIP_VERTICAL;

struct Dims {
	int width, height;
};

struct DimsF {
	float width, height;
};

struct Vector2 {
	float x, y;

	static Vector2 zero() {
		return { 0.0f, 0.0f };
	}
};

struct Vector3 {
	float x, y, z;

	static Vector3 zero() {
		return { 0.0f, 0.0f, 0.0f };
	}
};

struct Transform {
	Vector3 pos;
	double rotation;
	Flip flip;
	Vector2 scale;

	static Transform zero() {
		return { {0,0,0}, 0.0, NO_FLIP, {1.0f, 1.0f} };
	}
};

// Struktura Rect jest dwustronnie kompatybilna z SDL_FRect
struct Rect {
	float x, y, w, h;

	Rect(float x = 0, float y = 0, float w = 0, float h = 0)
		: x(x), y(y), w(w), h(h) {}

	Rect(const SDL_FRect& rect) : 
		x(rect.x), y(rect.y), w(rect.w), h(rect.h) {}

	Rect& operator=(const SDL_FRect& rect) {
		x = rect.x;
		y = rect.y;
		w = rect.w;
		h = rect.h;

		return *this;
	}

	SDL_FRect toSDL() const {
		return { x,y,w,h };
	}

	operator SDL_FRect() const {
		return toSDL();
	}

	inline bool contains(const Vector2& point) const {
		return (
			point.x >= x && point.x <= x + w
			&& point.y >= y && point.y <= y + h
		);
	}

	inline bool intersects(const Rect& other) const {
		return (
			x < other.x + other.w &&
			x + w > other.x &&
			y < other.y + other.h &&
			y + h > other.y
		);
	}
};

struct ColorRGBA {
	Uint8 r, g, b, a;

	static ColorRGBA black() {
		return { 0, 0, 0, 0xFF };
	}

	static ColorRGBA red() {
		return { 0xFF, 0, 0, 0xFF };
	}

	static ColorRGBA green() {
		return { 0, 0xFF, 0, 0xFF };
	}

	static ColorRGBA blue() {
		return { 0, 0, 0xFF, 0xFF };
	}
};

struct Sprite {
	SDL_Texture* texture;
	int width, height;
};

class GameObject {
protected:
	Transform transform;
	char* spriteKey;
	bool removalPending;
	Managers* mgs;
public:
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;

	GameObject(Managers* mgs, Transform tr = Transform::zero(), const char* key = nullptr);
	virtual ~GameObject();

	Transform& getTransform();
	void setTransform(Transform tr);

	Vector3& getPosition();
	void setPosition(Vector3 pos);

	Sprite* getSprite() const;
	void setSprite(const char* key);

	bool getRemovalFlag() const;
	void setRemovalFlag(bool flag);

	virtual void start() {};
	virtual void update(float dt) {};
	virtual void fixedUpdate(float fixed_dt) {};
	virtual void draw() {};
};

class Camera : public GameObject {
public:
	Camera(Managers* mgs, Vector3 pos = {0, 0, 500.0f});
	~Camera();

	Rect getViewport(Dims logDims);
	float getZoom();
};

struct AnimationClip {
	const char* spriteKey;
	SDL_Rect* frames;
	int frameCount;
	float frameDuration;
};