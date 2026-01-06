#pragma once

#include <cmath>
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
constexpr int MAX_TEXTSIZE = 256;
constexpr float BTN_PR_DUR = .15f;
constexpr int BTN_FC_BRD = 60;
constexpr float PULSE_SPD = 3.0f;
constexpr int PULSE_CHANGE = 60;
constexpr int PULSE_ALPHA_CHANGE = 60;

class Managers;

typedef SDL_RendererFlip Flip;

constexpr SDL_RendererFlip NO_FLIP = SDL_FLIP_NONE;
constexpr SDL_RendererFlip H_FLIP = SDL_FLIP_HORIZONTAL;
constexpr SDL_RendererFlip V_FLIP = SDL_FLIP_VERTICAL;

struct Dims {
	int width, height;
};

struct FDims {
	float width, height;

	operator Dims() const {
		return {(int)width, (int)height};
	}
};

struct DimsF {
	float width, height;
};

struct Vector2 {
	float x, y;

	static Vector2 zero() {
		return { 0.0f, 0.0f };
	}

	Vector2 operator+(const Vector2& other) const {
		return { x + other.x, y + other.y };
	}

	Vector2& operator+=(const Vector2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2 operator-(const Vector2& other) const {
		return { x - other.x, y - other.y };
	}

	Vector2& operator-=(const Vector2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2 operator*(float scalar) const {
		return { x * scalar, y * scalar };
	}

	Vector2& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	friend Vector2 operator*(float scalar, Vector2 vec) {
		return vec * scalar;
	}

	float length() const {
		return sqrt(x * x + y * y);
	}

	Vector2 normalized() const {
		float len = length();

		if (len == 0) return { 0, 0 };
		return *this * (1.0f / len);
	}
};

struct Vector3 {
	float x, y, z;

	static Vector3 zero() {
		return { 0.0f, 0.0f, 0.0f };
	}

	Vector3 operator+(const Vector3& other) const {
		return { x + other.x, y + other.y, z + other.z };
	}

	Vector3& operator+=(const Vector3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3 operator-(const Vector3& other) const {
		return { x - other.x, y - other.y, z - other.z };
	}

	Vector3& operator-=(const Vector3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vector3 operator*(float scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}

	Vector3& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	friend Vector3 operator*(float scalar, Vector3 vec) {
		return vec * scalar;
	}

	float length() const {
		return sqrt(x * x + y * y + z*z);
	}

	Vector3 normalized() const {
		float len = length();

		if (len == 0) return { 0, 0, 0 };
		return *this * (1.0f / len);
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

	Rect(const SDL_FRect& rect) 
		: x(rect.x), y(rect.y), w(rect.w), h(rect.h) {}

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

	static ColorRGBA white() {
		return { 0xFF, 0xFF, 0xFF, 0xFF };
	}

	static ColorRGBA transparent() {
		return { 0, 0, 0, 0 };
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

struct AnimationClip {
	int spriteKey;
	SDL_Rect* frames;
	int frameCount;
	float frameDuration;
};

struct Rigidbody {
	Vector3 vel;
	Vector3 prevPos;
	Vector3 currPos;
};

enum class InputType : Uint8 {
	KEYBOARD,
	MOUSE
};

struct InputBinding {
	InputType type;
	Uint8 key;

	bool operator==(const InputBinding other) const {
		return type == other.type && key == other.key;
	}
	bool operator!=(const InputBinding other) const {
		return type != other.type || key != other.key;
	}
};

typedef Array<InputBinding> ActionBinding;

struct BackgroundLayer {
	int spriteKey;
	float scrollFactor;
	FDims dims;
	int copies;
};



class Scene {
protected:
	Managers* mgs;
public:
	Scene(Managers* managers) : mgs(managers) {}
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	virtual ~Scene() { destroy(); }
	virtual void start() {}
	virtual void update(float dt) {}
	virtual void fixedUpdate(float fixed_dt) {}
	virtual void draw() {}
	virtual void destroy() {}
};

class GameScene : public Scene {
protected:
	ArrayList<BackgroundLayer> layers;
public:
	GameScene(Managers* managers) : Scene(managers) {}
	void addLayer(int spriteKey, float scrollFactor, float width, float height);
	void drawBackground();
	void drawLayer(BackgroundLayer& layer, float camX, Dims& logDims);
};

class GameObject {
protected:
	Transform transform;
	Rigidbody rb;
	bool removalPending;
	bool hasStarted;
	Managers* mgs;
public:
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;

	GameObject(Managers* mgs, Transform tr = Transform::zero());
	virtual ~GameObject();

	Transform& getTransform();
	void setTransform(Transform tr);

	Vector3& getPosition();
	void setPosition(Vector3 pos);

	Rigidbody& getRb();

	bool getRemovalFlag() const;
	void setRemovalFlag(bool flag);
	bool getStartedFlag() const;
	void setStartedFlag(bool flag);
	Vector3 getIPos();

	void drawShadow(int sh_key, float obj_width, FDims sh_dims);

	virtual void start() {};
	virtual void update(float dt) {};
	virtual void fixedUpdate(float fixed_dt) {};
	virtual void draw() {};
};

class SpriteObject : public GameObject {
protected:
	int spriteKey;
public:
	SpriteObject(Managers* mgs, Transform tr = Transform::zero(), int key = 0);
	virtual ~SpriteObject();

	Sprite* getSprite();
	void setSpriteKey(int key);

	virtual void draw() override;
};

class AnimatableObject : public GameObject {
protected:
	int currentAnimKey;
	int currentAnimFrame;
	float currentAnimTimer;
public:
	AnimatableObject(Managers* mgs, Transform tr = Transform::zero());
	virtual ~AnimatableObject();

	virtual void updateAnim(float dt);

	int getCurrentAnimKey();
	void setAnim(int key);
};

class Camera : public GameObject {
public:
	Camera(Managers* mgs, Vector3 pos = {0, 0, 500.0f});
	~Camera();

	Rect getViewport();
	float getZoom();
};
