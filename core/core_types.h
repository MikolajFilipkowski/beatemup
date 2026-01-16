#pragma once

#include <cmath>
#include "utils.h"

extern "C" {
#include"./SDL2/include/SDL.h"
}

inline constexpr SDL_Scancode FULLSCREEN_KEY = SDL_SCANCODE_F11;
inline constexpr SDL_Scancode EXIT_KEY = SDL_SCANCODE_ESCAPE;
inline constexpr SDL_Scancode DEBUG_KEY = SDL_SCANCODE_F3;

inline constexpr float DEFAULT_FOV = 90.0f;
inline constexpr float DEFAULT_CAM_Z = 500.0f;
inline constexpr float FIXED_DT = 0.01f;
inline constexpr float MAX_DT = 0.25f;
inline constexpr float FPS_CAP = 240.0f;
inline constexpr Uint32 MIN_FRAMETIME = (Uint32)(1000 / FPS_CAP);
inline constexpr Uint32 MAX_FRAMETIME = 500;
inline constexpr float FPS_INTERVAL = 1.0f;
inline constexpr int MAX_TEXTSIZE = 256;
inline constexpr int BTN_TEXTSIZE = 64;
inline constexpr float BTN_PR_DUR = .15f;
inline constexpr int BTN_FC_BRD = 60;
inline constexpr float PULSE_SPD = 3.0f;
inline constexpr int PULSE_CHANGE = 60;
inline constexpr int PULSE_ALPHA_CHANGE = 60;
inline constexpr float TEXT_CUR_BLINK_SPD = 7.0f;
inline constexpr float TEXT_CUR_SCALE = .1f;
inline constexpr float TEXT_CUR_PAD_LEFT = 5.0f;

inline constexpr int BAR_ANIM_TIME = 50;

inline constexpr Uint8 ACTOR_FLAG = 1U;

inline constexpr size_t MAX_ACT_NAME_LEN = 64;

class Managers;

typedef SDL_RendererFlip Flip;

inline constexpr SDL_RendererFlip NO_FLIP = SDL_FLIP_NONE;
inline constexpr SDL_RendererFlip H_FLIP = SDL_FLIP_HORIZONTAL;
inline constexpr SDL_RendererFlip V_FLIP = SDL_FLIP_VERTICAL;

inline constexpr int MAX_ERR_LEN = 512;

struct Vector2 {
	float x{ 0 }, y{ 0 };

	static Vector2 zero() {
		return { 0.0f, 0.0f };
	}

	Vector2 operator+(const Vector2& a_Other) const {
		return { x + a_Other.x, y + a_Other.y };
	}

	Vector2& operator+=(const Vector2& a_Other) {
		x += a_Other.x;
		y += a_Other.y;
		return *this;
	}

	Vector2 operator-(const Vector2& a_Other) const {
		return { x - a_Other.x, y - a_Other.y };
	}

	Vector2& operator-=(const Vector2& a_Other) {
		x -= a_Other.x;
		y -= a_Other.y;
		return *this;
	}

	Vector2 operator*(float a_Scalar) const {
		return { x * a_Scalar, y * a_Scalar };
	}

	Vector2& operator*=(float a_Scalar) {
		x *= a_Scalar;
		y *= a_Scalar;
		return *this;
	}

	friend Vector2 operator*(float a_Scalar, Vector2 a_Vector) {
		return a_Vector * a_Scalar;
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
	float x{ 0 }, y{ 0 }, z{ 0 };

	static Vector3 zero() {
		return { 0.0f, 0.0f, 0.0f };
	}

	Vector3 operator+(const Vector3& a_Other) const {
		return { x + a_Other.x, y + a_Other.y, z + a_Other.z };
	}

	Vector3& operator+=(const Vector3& a_Other) {
		x += a_Other.x;
		y += a_Other.y;
		z += a_Other.z;
		return *this;
	}

	Vector3 operator-(const Vector3& a_Other) const {
		return { x - a_Other.x, y - a_Other.y, z - a_Other.z };
	}

	Vector3& operator-=(const Vector3& a_Other) {
		x -= a_Other.x;
		y -= a_Other.y;
		z -= a_Other.z;
		return *this;
	}

	Vector3 operator*(float a_Scalar) const {
		return { x * a_Scalar, y * a_Scalar, z * a_Scalar };
	}

	Vector3& operator*=(float a_Scalar) {
		x *= a_Scalar;
		y *= a_Scalar;
		z *= a_Scalar;
		return *this;
	}

	friend Vector3 operator*(float a_Scalar, Vector3 a_Vector) {
		return a_Vector * a_Scalar;
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

struct Dims {
	int width{ 0 }, height{ 0 };
};

struct FDims {
	float width{ 0 }, height{ 0 };

	operator Dims() const {
		return { (int)width, (int)height };
	}

	FDims operator+(const Vector2& a_Vector) const {
		return { width + a_Vector.x, height + a_Vector.y };
	}

	FDims& operator+=(const Vector2& a_Vector) {
		width += a_Vector.x;
		height += a_Vector.y;
		return *this;
	}

	FDims operator-(const Vector2& a_Vector) const {
		return { width - a_Vector.x, height - a_Vector.y };
	}

	FDims& operator-=(const Vector2& a_Vector) {
		width -= a_Vector.x;
		height -= a_Vector.y;
		return *this;
	}
};

struct DimsF {
	float width{ 0 }, height{ 0 };
};

struct Transform {
	Vector3 pos{};
	double rotation{ 0 };
	Flip flip{ NO_FLIP };
	Vector2 scale{1.0f, 1.0f};

	static Transform zero() {
		return { {0,0,0}, 0.0, NO_FLIP, {1.0f, 1.0f} };
	}
};

// Struktura Rect jest dwustronnie kompatybilna z SDL_FRect
struct Rect {
	float x{ 0 }, y{ 0 }, w{ 0 }, h{ 0 };

	Rect() = default;

	Rect(float a_X, float a_Y, float a_W, float a_H)
		: x(a_X), y(a_Y), w(a_W), h(a_H) {}

	Rect(const SDL_FRect& a_Rect) 
		: x(a_Rect.x), y(a_Rect.y), w(a_Rect.w), h(a_Rect.h) {}

	Rect& operator=(const SDL_FRect& a_Rect) {
		x = a_Rect.x;
		y = a_Rect.y;
		w = a_Rect.w;
		h = a_Rect.h;

		return *this;
	}

	SDL_FRect toSDL() const {
		return { x,y,w,h };
	}

	operator SDL_FRect() const {
		return toSDL();
	}

	inline bool contains(const Vector2& a_Point) const {
		return (
			a_Point.x >= x && a_Point.x <= x + w
			&& a_Point.y >= y && a_Point.y <= y + h
		);
	}

	inline bool intersects(const Rect& a_Other) const {
		return (
			x < a_Other.x + a_Other.w &&
			x + w > a_Other.x &&
			y < a_Other.y + a_Other.h &&
			y + h > a_Other.y
		);
	}

	inline Vector2 overlap(const Rect& a_Other) const {
		if (!intersects(a_Other)) 
			return Vector2();
		return {
			(x < a_Other.x) ? (x + w - a_Other.x) : (a_Other.x + a_Other.w - x),
			(y < a_Other.y) ? (y + h - a_Other.y) : (a_Other.y + a_Other.h - y)
		};
	}
};

struct Cuboid {
	float x{ 0 }, y{ 0 }, z{ 0 };
	float w{ 0 }, h{ 0 }, d{ 0 };

	inline bool contains(const Vector3& a_Point) const {
		return (
			a_Point.x >= x && a_Point.x <= x + w
			&& a_Point.y >= y && a_Point.y <= y + h
			&& a_Point.z >= z && a_Point.z <= z + d
		);
	}

	inline bool intersects(const Cuboid& a_Other) const {
		return (
			x < a_Other.x + a_Other.w &&
			x + w > a_Other.x &&
			y < a_Other.y + a_Other.h &&
			y + h > a_Other.y &&
			z < a_Other.z + a_Other.d &&
			z + d > a_Other.z
		);
	}

	inline Vector3 overlap(const Cuboid& a_Other) const {
		if (!intersects(a_Other))
			return Vector3();
		return {
			(x < a_Other.x) ? (x + w - a_Other.x) : (a_Other.x + a_Other.w - x),
			(y < a_Other.y) ? (y + h - a_Other.y) : (a_Other.y + a_Other.h - y),
			(z < a_Other.z) ? (z + d - a_Other.z) : (a_Other.z + a_Other.d - z)
		};
	}
};

struct ColorRGBA {
	Uint8 r{ 0 }, g{ 0 }, b{ 0 }, a{ 0xFF };

	constexpr ColorRGBA() {}
	constexpr ColorRGBA(Uint8 a_R, Uint8 a_G, Uint8 a_B, Uint8 a_A)
		: r(a_R), g(a_G), b(a_B), a(a_A) {}

	static constexpr ColorRGBA black() {
		return { 0, 0, 0, 0xFF };
	}

	static constexpr ColorRGBA white() {
		return { 0xFF, 0xFF, 0xFF, 0xFF };
	}

	static constexpr ColorRGBA transparent() {
		return { 0, 0, 0, 0 };
	}

	static constexpr ColorRGBA red() {
		return { 0xFF, 0, 0, 0xFF };
	}

	static constexpr ColorRGBA green() {
		return { 0, 0xFF, 0, 0xFF };
	}

	static constexpr ColorRGBA blue() {
		return { 0, 0, 0xFF, 0xFF };
	}
};

struct Sprite {
	SDL_Texture* texture{};
	int width{ 0 }, height{ 0 };
};

struct AnimationClip {
	int spriteKey{ 0 };
	SDL_Rect* frames{};
	int frameCount{0};
};

struct Outline {
	ColorRGBA color{ColorRGBA::transparent()};
	float size{0.0f};

	constexpr Outline() = default;
	constexpr Outline(ColorRGBA a_Color, float a_Size) 
		: color(a_Color), size(a_Size) {}
	constexpr Outline(const Outline& a_Other) 
		: color(a_Other.color), size(a_Other.size) {}
};

struct Font {
	int key;
	int chSize;
	float scale;
	float spacing;
	ColorRGBA color;
	Outline outline;
	float baseline;

	constexpr Font() : key(0), chSize(0), scale(0),
		spacing(0), color(ColorRGBA::black()), outline(Outline()), baseline(0) {}

	constexpr Font(int a_Key, int a_ChSize, float a_Scale, float a_Spacing, ColorRGBA a_Color, Outline a_Outline, float a_Baseline) 
		: key(a_Key), chSize(a_ChSize), scale(a_Scale),spacing(a_Spacing), color(a_Color), outline(a_Outline), baseline(a_Baseline) {}

	constexpr Font(int a_Key, int a_ChSize, float a_Scale, float a_Spacing, ColorRGBA a_Color)
		: key(a_Key), chSize(a_ChSize), scale(a_Scale), spacing(a_Spacing), color(a_Color), outline(Outline()), baseline(0) {
	}

	float getWidth(int a_CharCount) const {
		return a_CharCount * chSize * scale * spacing;
	}
};

struct Rigidbody {
	Vector3 vel{};
	Vector3 prevPos{};
	Vector3 currPos{};
	float mass{ 1.0f };
};

enum class InputType : Uint8 {
	KEYBOARD,
	MOUSE
};

struct InputBinding {
	InputType type;
	Uint8 key;

	bool operator==(const InputBinding a_Other) const {
		return type == a_Other.type && key == a_Other.key;
	}
	bool operator!=(const InputBinding a_Other) const {
		return type != a_Other.type || key != a_Other.key;
	}
};

typedef Array<InputBinding> InputBinds;

struct ActionFrame {
	Rect hitbox{};
	Rect hurtbox{};
	Vector3 vel{};
	float duration{ 0 };
	float damage{ 0 };

	ActionFrame() = default;
};

class ActionData {
private:
	Array<int>* m_Sequence{};
	Array<ActionFrame>* m_Frames{};
	float m_TotalDuration{ 0 };
	bool m_IsAttack{ false };
	Vector2 m_Range{};
public:
	char name[MAX_ACT_NAME_LEN]{};
	int priority{ 0 };
	int inputWindow{ 0 };
	Uint8 conditions{ 0U };
	bool interruptible{ false };
	bool canMove{ false };
	bool shouldLoop{ false };
	Uint8 owner{ 0U };

	ActionData() = default;
	ActionData(int a_Prio, int a_Win, int a_Cond, bool a_Inter, 
		int a_Owner, Array<int>* a_Seq, Array<ActionFrame>* a_Frames
	);
	~ActionData();
	ActionData(const ActionData&) = delete;
	ActionData& operator=(const ActionData&) = delete;

	void sealData();

	void setSequence(Array<int>* a_Seq);
	void setFrames(Array<ActionFrame>* a_Frames);
	Array<int>* getSequence() const;
	Array<ActionFrame>* getFrames() const;

	float getTotalDuration() const;
	bool isAttack() const;
	Vector2 getAttackRange() const;
};

struct ActiveAction {
	int actionKey;
	int currentFrame;
	bool hasHit;
};

struct BackgroundLayer {
	int spriteKey{ 0 };
	float scrollFactor{ 1.0f };
	FDims dims{};
	int copies{ 0 };
};

class Scene {
protected:
	Managers* m_Mgs;
public:
	Scene(Managers* a_Managers) : m_Mgs(a_Managers) {}
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	virtual ~Scene() { destroy(); }
	virtual void start() {}
	virtual void update(float a_Dt) {}
	virtual void fixedUpdate(float a_FixedDt) {}
	virtual void draw() {}
	virtual void destroy() {}
};

class GameScene : public Scene {
protected:
	ArrayList<BackgroundLayer> m_Layers;
public:
	GameScene(Managers* a_Managers) : Scene(a_Managers) {}
	void addLayer(int a_SpriteKey, float a_ScrollFactor, float a_Width, float a_Height);
	void drawBackground();
	void drawLayer(BackgroundLayer& a_Layer, float a_CamX, Dims& a_LogDims);
};

class GameObject {
protected:
	Managers* m_Mgs{};
	Transform m_Transform{};
	Rigidbody m_Rb{};
	bool m_RemovalPending{ false };
	bool m_HasStarted{ false };
public:
	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;

	GameObject(Managers* a_Managers, Transform a_Transform = Transform::zero());
	virtual ~GameObject();

	virtual Uint8 getType() const;

	Transform& getTransform();
	void setTransform(Transform a_Transform);

	Vector3& getPosition();
	void setPosition(Vector3 a_Pos);

	Rigidbody& getRb();

	bool getRemovalFlag() const;
	void setRemovalFlag(bool a_Flag);
	bool getStartedFlag() const;
	void setStartedFlag(bool a_Flag);
	Vector3 getIPos();

	virtual Cuboid getCollBox();

	void drawShadow(int a_ShKey, float a_ObjWidth, FDims a_ShDims);

	virtual void start() {};
	virtual void update(float a_Dt) {};
	virtual void fixedUpdate(float a_FixedDt) {};
	virtual void postFixedUpdate(float a_FixedDt) {};
	virtual void draw() {};
};

class SpriteObject : public GameObject {
protected:
	int m_SpriteKey{ 0 };
public:
	SpriteObject(Managers* a_Managers, Transform a_Transform = Transform::zero(), int a_Key = 0);
	virtual ~SpriteObject();

	Sprite* getSprite();
	void setSpriteKey(int a_Key);

	virtual void draw() override;
};

class Camera : public GameObject {
public:
	Camera(Managers* a_Managers, Vector3 a_Pos = {0, 0, 500.0f});
	~Camera();

	Rect getViewport();
	float getZoom();
};
