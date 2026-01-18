#pragma once

#include "../core/core.h"

#define ASSETS "game/assets/"

inline constexpr int MAX_PLY_LEN = 10;

inline constexpr float GRAVITY = 4500.0f;
inline constexpr float PLY_SPEED = 450.0f;
inline constexpr float ENEMY_SPEED = 300.0f;
inline constexpr float FRICTION = .2f;
inline constexpr float MIN_SPEED = .5f;
inline constexpr float Z_AXIS_MUL = .6f;
inline constexpr float IN_AIR_MUL = .9f;

inline constexpr float ACTOR_SCALE = 2.5f;
inline constexpr float AUTUMN_SCALE_Y = 2.7f;

inline constexpr float PLY_POS_X = 200.0f;
inline constexpr float PLY_POS_Z = 350.0f;

inline constexpr float CAM_POS_Z = 500.0f;

inline constexpr float CAMERA_LERP = .2f;
inline constexpr float CAMERA_THR = .15f;

inline constexpr float IC_SIZE = 50.0f;
inline constexpr float IC_OFF_X = -64.0f;
inline constexpr float IC_OFF_Y = 0.0f;

inline constexpr float INFO_POSX = 5.0f;
inline constexpr float INFO_POSY = 3.0f;

inline constexpr float MENU_BTN_H = 56.0f;
inline constexpr float MENU_BTN_GAP = 8.0f;

inline constexpr float BTN_X_MUL = .35f;
inline constexpr float BTN_Y_MUL = .38f;
inline constexpr float BTN_W_MUL = .3f;

inline constexpr int DEF_BUFFER_SIZE = 16;
inline constexpr int BUFFER_CLR_DECAY = 60;
inline constexpr int BUFFER_DECAY = 10;
inline constexpr int LOW_PRIO = 20;
inline constexpr int MIN_FRTIME_LOW_PRIO = 2;

inline constexpr int SHORT_TEXT_MAXLEN = 64;
inline constexpr int UINT8_MAXLEN = 4;
inline constexpr int INT32_MAXLEN = 10;

inline constexpr SDL_Scancode NEW_GAME_KEY = SDL_SCANCODE_N;

inline constexpr float DEBUG_BUFF_Y = 120.0f;

inline constexpr float DEF_HP = 100.0f;

inline constexpr FDims SHADOW_DIMS = { 96, 32 };
inline constexpr float AFTER_HIT_INV = 0.25f;

inline constexpr float MAX_DZ_HIT = 35.0f;
inline constexpr float COLLBOX_Z_SIZE = 20.0f;
inline constexpr float COLLBOX_W_MUL = 0.9f;

inline constexpr float STUN_DURATION = .1f;
inline constexpr float DEATH_DURATION = 1.0f;

inline constexpr float KNOCKBACK_X = 1850.0f;
inline constexpr float KNOCKBACK_Y = -350.0f;

inline constexpr float AI_ATT_MARGIN = .1f;
inline constexpr int AI_Z_LAYERS = 5;
inline constexpr float AI_Z_SEPARATION = 10.0f;

inline constexpr float COMBO_DISPLAY_TIME = 2.0f;
inline constexpr float COMBO_DECAY = 5.0f;
inline constexpr float BASE_SCORE_MUL = 10.0f;
inline constexpr float HITS_MUL = 0.01f;

inline constexpr int GODLIKE_THR = 100U;
inline constexpr float GODLIKE_MUL = 5.0f;
inline constexpr const char* GODLIKE_TXT = "GODLIKE!";

inline constexpr int BRILLIANT_THR = 50U;
inline constexpr float BRILLIANT_MUL = 3.0f;
inline constexpr const char* BRILLIANT_TXT = "BRILLIANT!";

inline constexpr int GREAT_THR = 25U;
inline constexpr float GREAT_MUL = 2.0f;
inline constexpr const char* GREAT_TXT = "GREAT!";

inline constexpr int NICE_THR = 10U;
inline constexpr float NICE_MUL = 1.5f;
inline constexpr const char* NICE_TXT = "NICE!";

inline constexpr const char* HITS_LIT = "Hits";
inline constexpr float COMBO_XOFF = -12.0f;

inline constexpr Uint8 WIN_FLAG = 1;

inline constexpr const char* PLACEHOLDER_TEXT = "Pseudonim";
inline constexpr ColorRGBA PLACEHOLDER_CLR = { 0x77, 0x77, 0x77, 0xFF };
inline constexpr ColorRGBA SCROLL_AREA_CLR = { 0x33, 0x33, 0x33, 0x99 };

inline constexpr float BG_DEF_W = 1280.0f;
inline constexpr float BG_DEF_H = 720.0f;
inline constexpr float BG_DEF_SPD = 1.0f;
inline constexpr float BG_BLD_SPD = .2f;
inline constexpr float BG_SKY_SPD = .1f;
inline constexpr float BG_ROAD_W = 1280.0f;

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
		CAMERA = (1U << 5),
		ALL = 0xFFU
	};
};

namespace EnemyType {
	enum : Uint8 {
		DOYLE = 1,
		AUTUMN = 2,
	};
}

namespace ObstacleType {
	enum : Uint8 {
		BARREL = 1,
	};
}

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
		BG_ELEMENT1,
		BG_ELEMENT2,
		BG_BUILDINGS,
		BG_ROAD,
		BG_WALL1,
		BG_WALL2,
		BG_SKY,
		HB_FRAME,
		HB_FILL,
		HB_GHOST,
		BARREL,

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
		PLY_JUMP_KICK,
		PLY_JUMP_PUNCH,
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
	inline constexpr ColorRGBA yellow = {0xFF, 0xFF, 0, 0xFF};
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
		JUMP_KICK,
		JUMP_PUNCH
	};
}

namespace SceneID {
	enum {
		NONE = 0,
		MENU,
		LEVEL,
	};
}

