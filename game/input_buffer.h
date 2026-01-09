#pragma once

#include "../core/core.h"
#include "config.h"

struct InputEntry {
	int bind;
	int worldFrame;
};

class InputBuffer {
private:
	Managers* mgs;
	CircularQueue<InputEntry> buffer;
	Font debFont;
public:
	InputBuffer(Managers* mgs);
	int count() const;
	bool matches(int actionId, int worldFrame) const;
	void update();
	void clear();
	void drawBuffer();

	static char bindToChar(int bind);
};