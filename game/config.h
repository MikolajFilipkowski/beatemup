#pragma once

#include "../core/core.h"

#define ASSETS "game/assets/"

inline constexpr float GRAVITY = 4500.0f;
inline constexpr float PLY_SPEED = 450.0f;
inline constexpr float JUMP_FORCE = -2000.0f;
inline constexpr float Z_AXIS_MUL = .6f;
inline constexpr float IN_AIR_MUL = .85f;

inline constexpr float CAMERA_LERP = .2f;
inline constexpr float CAMERA_THR = .25f;

inline constexpr float IC_SIZE = 50.0f;
inline constexpr float IC_OFF_X = -64.0f;
inline constexpr float IC_OFF_Y = 0.0f;

inline constexpr float MENU_BTN_H = 56.0f;
inline constexpr float MENU_BTN_GAP = 8.0f;

inline constexpr int DEF_BUFFER_SIZE = 16;
inline constexpr int BUFFER_CLR_DECAY = 60;

inline constexpr SDL_Scancode NEW_GAME_KEY = SDL_SCANCODE_N;

inline constexpr float DEBUG_BUFF_Y = 120.0f;

inline constexpr float DEF_HP = 100.0f;

inline constexpr FDims SHADOW_DIMS = { 96, 32 };

inline constexpr float MAX_DZ_HIT = 20.0f;
inline constexpr float AFTER_HIT_INV = 0.15f;

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
	};
};

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
		PLY_HEAVY_ATT,
		PLY_LIGHT_ATT,
		PLY_WHEEL_PUNCH,
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
		LIGHT_ATTACK,
		HEAVY_ATTACK,
		WHEEL_PUNCH
	};
}

namespace SceneID {
	enum {
		NONE = 0,
		MENU,
		LEVEL,
	};
}

namespace Entities {
	enum : Uint8 {
		NONE = 0U,
		PLAYER = 1U << 0,
		DOYLE = 1U << 1,
		AUTUMN = 1U << 2,
		ALL = 0xFFU
	};
}