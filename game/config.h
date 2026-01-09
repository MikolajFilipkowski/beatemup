#pragma once

#include "../core/core.h"

#define ASSETS "game/assets/"

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

inline constexpr SDL_Scancode DEBUG_KEY = SDL_SCANCODE_F3;
inline constexpr float DEBUG_BUFF_Y = 120.0f;

inline constexpr float DEF_HP = 100.0f;


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
		PLY_ATTACK_1,
		PLY_ATTACK_2,
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

namespace ActionBind {
	enum {
		NONE = 0,
		JUMP,
		LEFT,
		RIGHT,
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