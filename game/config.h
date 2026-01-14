#pragma once

#include "../core/core.h"

#define ASSETS "game/assets/"

inline constexpr float GRAVITY = 4500.0f;
inline constexpr float PLY_SPEED = 450.0f;
inline constexpr float ENEMY_SPEED = 300.0f;
inline constexpr float FRICTION = .2f;
inline constexpr float MIN_SPEED = .5f;
inline constexpr float Z_AXIS_MUL = .6f;
inline constexpr float IN_AIR_MUL = .9f;

inline constexpr float CAMERA_LERP = .2f;
inline constexpr float CAMERA_THR = .25f;

inline constexpr float IC_SIZE = 50.0f;
inline constexpr float IC_OFF_X = -64.0f;
inline constexpr float IC_OFF_Y = 0.0f;

inline constexpr float MENU_BTN_H = 56.0f;
inline constexpr float MENU_BTN_GAP = 8.0f;

inline constexpr int DEF_BUFFER_SIZE = 16;
inline constexpr int BUFFER_CLR_DECAY = 60;
inline constexpr int BUFFER_DECAY = 10;
inline constexpr int LOW_PRIO = 20;
inline constexpr int MIN_FRTIME_LOW_PRIO = 2;

inline constexpr SDL_Scancode NEW_GAME_KEY = SDL_SCANCODE_N;

inline constexpr float DEBUG_BUFF_Y = 120.0f;

inline constexpr float DEF_HP = 100.0f;

inline constexpr FDims SHADOW_DIMS = { 96, 32 };
inline constexpr float AFTER_HIT_INV = 0.25f;

inline constexpr float MAX_DZ_HIT = 30.0f;
inline constexpr float COLLBOX_Z_SIZE = 20.0f;
inline constexpr float COLLBOX_W_MUL = 0.9f;

inline constexpr float DEATH_DURATION = 1.0f;

inline constexpr float KNOCKBACK_X = 1850.0f;
inline constexpr float KNOCKBACK_Y = -350.0f;

inline constexpr float AI_ATT_MARGIN = .2f;
inline constexpr int AI_Z_LAYERS = 5;
inline constexpr float AI_Z_SEPARATION = 10.0f;

enum class FacingDir {
	LEFT,
	RIGHT
};

// Dziala podobnie do enum class, ale automatycznie rzutuje na inta

namespace ObjectType {
	enum : Uint8 {
		DEFAULT = 0U,
		ACTOR = 1U << 0,
		PLAYER = (1U << 1) | ACTOR,
		ENEMY = (1U << 2) | ACTOR,
		DOYLE = (1U << 3) | ENEMY,
		AUTUMN = (1U << 4) | ENEMY,
		ALL = 0xFFU
	};
};

namespace ActionCond {
	enum : Uint8 {
		NONE = 0U,
		GROUND_ONLY = 1U << 0,
		AIR_ONLY = 1U << 1,
	};
}

namespace RES {
	enum {
		NONE = 0,
		CH_8,
		CH_16,
		CH_32,
		CH_64,
		CIRCLE,
		SHADOW,
		PUNCH,
		UI_BIG_FRAME,
		UI_SMALL_FRAME,
		UI_BUTTON,
		UI_TEXT_INPUT,
		MENU_BOXES,
		MENU_WHEELS,
		MENU_BUILDINGS,
		MENU_ROAD,
		MENU_WALL1,
		MENU_WALL2,
		MENU_SKY,
		HB_FRAME,
		HB_FILL,
		HB_GHOST,

		ANIMS_OFFSET,
	};

	enum {
		PLY_IDLE = (int)ANIMS_OFFSET,
		PLY_WALK,
		PLY_JUMP,
		PLY_HURT,
		PLY_DEATH,
		PLY_DASH,
		PLY_HEAVY_ATT,
		PLY_LIGHT_ATT,
		PLY_WHEEL_PUNCH,
		PLY_HIGH_KICK,
		DOYLE_IDLE,
		DOYLE_WALK,
		DOYLE_HURT,
		DOYLE_DEATH,
		DOYLE_DASH,
		DOYLE_HEAVY_ATT,
		DOYLE_LIGHT_ATT,
		DOYLE_WHEEL_PUNCH,
		AUTUMN_IDLE,
		AUTUMN_WALK,
		AUTUMN_HURT,
		AUTUMN_DEATH,
		AUTUMN_DASH,
		AUTUMN_HEAVY_ATT,
		AUTUMN_LIGHT_ATT,
		AUTUMN_WHEEL_PUNCH,
	};
}

namespace Colors {
	inline constexpr ColorRGBA black = ColorRGBA::black();
	inline constexpr ColorRGBA white = ColorRGBA::white();
	inline constexpr ColorRGBA blue = ColorRGBA::blue();
	inline constexpr ColorRGBA red = ColorRGBA::red();
	inline constexpr ColorRGBA green = ColorRGBA::green();
	inline constexpr ColorRGBA vividBlue = { 0x1A, 0x43, 0xD9, 0xFF };
	inline constexpr ColorRGBA gainsboro = { 0xDD, 0xDD, 0xDD, 0xFF };
	inline constexpr ColorRGBA forestGreen = { 0x2E, 0x6F, 0x40, 0xFF };
}

namespace InputBind {
	enum {
		NONE = 0,
		JUMP,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		ACT_X,
		ACT_Y,

		INPUT_COUNT
	};
}

namespace ActionBind {
	enum {
		NONE = 0,
		JUMP,
		FORWARD,
		BACKWARD,
		UP,
		DOWN,
		ACT_X,
		ACT_Y,

		BIND_COUNT
	};
}

namespace Actions {
	enum {
		NONE,
		IDLE,
		WALK,
		JUMP,
		HURT,
		DEATH,
		DASH_FORWARD,
		DASH_BACKWARD,
		LIGHT_ATTACK,
		HEAVY_ATTACK,
		WHEEL_PUNCH,
		HIGH_KICK,
	};
}

namespace SceneID {
	enum {
		NONE = 0,
		MENU,
		LEVEL,
	};
}