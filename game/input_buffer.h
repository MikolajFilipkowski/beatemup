#pragma once

#include "../core/core.h"
#include "config.h"

class Player;

struct InputEntry {
	int logicalBind;
	int physicalBind;
	int worldFrame;
};

class InputBuffer {
private:
	Managers* m_Mgs{};
	CircularQueue<InputEntry> m_Buffer{ DEF_BUFFER_SIZE };
	Font m_DebugFont;
	Font m_SmallDebugFont;
	Player* m_Player;
public:
	InputBuffer(Managers* a_Managers, Player* a_Player);
	int count() const;
	bool matches(int a_ActionId, int a_WorldFrame) const;
	void update();
	void clear();
	void drawBuffer();

	static char bindToChar(int a_Bind);
};