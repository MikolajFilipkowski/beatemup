#include "input_buffer.h"

#include <cstdio>

InputBuffer::InputBuffer(Managers* mgs) : mgs(mgs), buffer(DEF_BUFFER_SIZE) {
	debFont = { RES::CH_32, 32, 1.0, 1.0, {0xDD, 0xDD, 0xDD, 0xFF}, Outline(), 12 };
}

int InputBuffer::count() const {
	return buffer.count();
}

bool InputBuffer::matches(int actionId, int worldFrame) const
{
	if (actionId <= 0 || count() == 0) 
		return false;

	ActionData* act = mgs->object->getAction(actionId);
	if (act->sequence == nullptr || act->sequence->count() == 0)
		return false;

	Array<int>*& seq = act->sequence;
	int seqId = seq->count() - 1;
	
	const InputEntry& lastEntry = buffer.peekAt(count() - 1);
	if (worldFrame - lastEntry.worldFrame > act->inputWindow)
		return false;

	for (int i = 0; i < buffer.count(); i++) {
		const InputEntry& entry = buffer.peekAt(count() - 1 - i);

		if (entry.bind != seq->get(seqId)) 
			continue;
		if (worldFrame - entry.worldFrame > act->inputWindow)
			return false;

		seqId--;
		if (seqId < 0) return true;
	}

	return false;
}

void InputBuffer::update()
{
	int wf = mgs->time->getWorldFrame();

	for (int i = 1; i < ActionBind::BIND_COUNT; i++) {
		if (mgs->input->getActionDown(i)) {
			buffer.push({ i, wf });
		}
	}
}

void InputBuffer::clear()
{
	buffer.clear();
}

void InputBuffer::drawBuffer()
{
	float size = mgs->display->getLogWidth() / (float)(DEF_BUFFER_SIZE + 2);
	int wf = mgs->time->getWorldFrame();

	for (int i = 0; i < count(); i++) {
		const InputEntry& entry = buffer.peekAt(i);
		char ch = bindToChar(entry.bind);
		if (ch == '\0') return;

		char txt[2] = { ch, '\0' };

		const float x = size * (i + 1);
		const float y = DEBUG_BUFF_Y;

		const float ch_x = (x + size / 2.0f) - (debFont.chSize * debFont.scale) / 2.0f;
		const float ch_y = (y + size / 2.0f) - ((debFont.chSize - debFont.baseline) * debFont.scale) / 2.0f;

		Sprite* spr = mgs->sprite->get(RES::CIRCLE);
		mgs->display->setSpriteModColor(spr, ColorRGBA::black());
		mgs->display->drawSprite(RES::CIRCLE, { x, y }, { size, size });
		
		ColorRGBA clr = Colors::vividBlue;

		int diff = wf - entry.worldFrame;
		int dist = BUFFER_CLR_DECAY - diff;
		if (dist > 0) {
			clr.r = clamp(clr.r + dist, 0, 0xFF);
			clr.g = clamp(clr.g + dist, 0, 0xFF);
			clr.b = clamp(clr.b + dist, 0, 0xFF);
		}

		mgs->display->setSpriteModColor(spr, clr);

		mgs->display->drawSprite(RES::CIRCLE, { x + 3, y + 3 }, { size - 6, size - 6 });
		mgs->display->drawString(RES::CH_32, { ch_x, ch_y }, txt, debFont);
		//mgs->display->drawRect({ ch_x, ch_y }, { debFont.chSize * debFont.scale, debFont.chSize * debFont.scale }, ColorRGBA::red(), 2);
		mgs->display->setSpriteModColor(spr, ColorRGBA::white());
	}
}

char InputBuffer::bindToChar(int bind)
{
	switch (bind) {
	case ActionBind::LEFT: return 'L';
	case ActionBind::RIGHT: return 'R';
	case ActionBind::UP: return 'U';
	case ActionBind::DOWN: return 'D';
	case ActionBind::JUMP: return 'J';
	case ActionBind::ACT_X: return 'X';
	case ActionBind::ACT_Y: return 'Y';
	default: return '\0';
	}
}