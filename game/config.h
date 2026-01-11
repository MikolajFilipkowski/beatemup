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


// Dziala podobnie do enum class, ale automatycznie rzutuje na inta

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

		ANIMS_OFFSET,
	};

	enum {
		PLY_IDLE = (int)ANIMS_OFFSET,
		PLY_WALK,
		PLY_JUMP,
		PLY_HEAVY_ATT,
		PLY_LIGHT_ATT,
		PLY_ATTACK_3,
	};
}

namespace Colors {
	inline constexpr ColorRGBA black = ColorRGBA::black();
	inline constexpr ColorRGBA white = ColorRGBA::white();
	inline constexpr ColorRGBA blue = ColorRGBA::blue();
	inline constexpr ColorRGBA red = ColorRGBA::red();
	inline constexpr ColorRGBA green = ColorRGBA::green();
	inline constexpr ColorRGBA vividBlue = { 0x1A, 0x43, 0xD9, 0xFF };
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
		HEAVY_ATTACK
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
	enum {
		NONE = 0,
		PLAYER = 1 << 0,
		ENEMY_1 = 1 << 1,
		ENEMY_2 = 1 << 2,
		ALL = 0xFF
	};
}