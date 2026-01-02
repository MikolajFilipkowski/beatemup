#pragma once

// Dziala podobnie do enum class, ale automatycznie rzutuje na inta

namespace RES {
	enum {
		CH_8 = 1,
		CH_16,
		CH_32,
		CH_64,
		BG,

		ANIMS_OFFSET,
	};

	enum {
		PLY_IDLE = (int)ANIMS_OFFSET,
		PLY_RUN,
		PLY_WALK,
		PLY_JUMP,
	};
}

namespace Action {
	enum {
		JUMP = 1,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		ACT_X,
		ACT_Y
	};
}