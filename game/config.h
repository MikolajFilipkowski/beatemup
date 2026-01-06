#pragma once

#define ASSETS "game/assets/"

constexpr float CAMERA_LERP = .2f;
constexpr float CAMERA_THR = .25f;

constexpr float IC_SIZE = 50.0f;
constexpr float IC_OFF_X = -64.0f;
constexpr float IC_OFF_Y = 8.0f;

constexpr float MENU_BTN_H = 56.0f;
constexpr float MENU_BTN_GAP = 8.0f;

// Dziala podobnie do enum class, ale automatycznie rzutuje na inta

namespace RES {
	enum {
		NONE = 0,
		CH_8,
		CH_16,
		CH_32,
		CH_64,
		SHADOW,
		PUNCH_ICON,
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
		PLY_RUN,
		PLY_WALK,
		PLY_JUMP,
		PLY_ATTACK_1,
		PLY_ATTACK_2,
		PLY_ATTACK_3,
	};
}

namespace Action {
	enum {
		NONE = 0,
		JUMP,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		ACT_X,
		ACT_Y
	};
}

namespace SceneID {
	enum {
		NONE = 0,
		MENU,
		LEVEL,
	};
}