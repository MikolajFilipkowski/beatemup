#pragma once

// Dziala podobnie do enum class, ale automatycznie rzutuje na inta

namespace RES {
	enum {
		NONE = 0,
		CH_8,
		CH_16,
		CH_32,
		CH_64,
		LVL_BOXES,
		LVL_WHEELS,
		LVL_BUILDINGS,
		LVL_ROAD,
		LVL_WALL1,
		LVL_WALL2,
		LVL_SKY,

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