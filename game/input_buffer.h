#pragma once

#include "../core/core.h"
#include "config.h"

class Player;

enum class InputState : Uint8 {
	NEW,
	IN_USE,
	USED
};

struct InputEntry {
	int logicalBind;
	int physicalBind;
	int worldFrame;
	InputState state;
};

class InputBuffer {
private:
	Managers* m_Mgs{};
	CircularQueue<InputEntry> m_Buffer{ DEF_BUFFER_SIZE };
	Font m_DebugFont;
	Font m_SmallDebugFont;
	Player* m_Player;
	bool m_FirstUpdate{ true };
public:
	InputBuffer(Managers* a_Managers, Player* a_Player);
	int count() const;
	bool matches(int a_ActionId, int a_WorldFrame, int* a_UsedEntries);
	int getNextAct();
	void update();
	void flagUsed();
	void clear();
	void drawBuffer();

	static char bindToChar(int a_Bind);
};