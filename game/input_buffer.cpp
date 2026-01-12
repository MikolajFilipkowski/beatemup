#include "input_buffer.h"

#include <cstdio>
#include "./gameObjects/player.h"

InputBuffer::InputBuffer(Managers* a_Managers, Player* a_Player) : m_Mgs(a_Managers), m_Player(a_Player) {
	m_DebugFont = { RES::CH_32, 32, 1.0, 1.0, Colors::gainsboro, Outline(), 12 };
	m_SmallDebugFont = { RES::CH_16, 16, .5, 1.0, Colors::gainsboro, Outline(), 0 };
}

int InputBuffer::count() const {
	return m_Buffer.count();
}

bool InputBuffer::matches(int a_ActionId, int a_WorldFrame) const
{
	if (a_ActionId <= 0 || count() == 0) 
		return false;

	ActionData* act = m_Mgs->object->getAction(a_ActionId);
	if (act->getSequence() == nullptr || act->getSequence()->count() == 0)
		return false;

	Array<int>* seq = act->getSequence();
	int seqId = seq->count() - 1;
	
	const InputEntry& lastEntry = m_Buffer.peekAt(count() - 1);
	if (a_WorldFrame - lastEntry.worldFrame > act->inputWindow)
		return false;

	for (int i = 0; i < m_Buffer.count(); i++) {
		const InputEntry& entry = m_Buffer.peekAt(count() - 1 - i);

		if (entry.logicalBind != seq->get(seqId)) 
			continue;
		if (a_WorldFrame - entry.worldFrame > act->inputWindow)
			return false;

		seqId--;
		if (seqId < 0) return true;
	}

	return false;
}

void InputBuffer::update()
{
	int wf = m_Mgs->time->getWorldFrame();

	FacingDir dir = m_Player->getFacingDir();

	if (m_Mgs->input->getInputDown(InputBind::RIGHT)) {
		int bind = (dir == FacingDir::RIGHT) ? ActionBind::FORWARD : ActionBind::BACKWARD;
		m_Buffer.push({ bind, InputBind::RIGHT, wf });
	}

	if (m_Mgs->input->getInputDown(InputBind::LEFT)) {
		int bind = (dir == FacingDir::LEFT) ? ActionBind::FORWARD : ActionBind::BACKWARD;
		m_Buffer.push({ bind, InputBind::LEFT, wf });
	}

	if (m_Mgs->input->getInputDown(InputBind::UP))
		m_Buffer.push({ ActionBind::UP, InputBind::UP, wf });
	if (m_Mgs->input->getInputDown(InputBind::DOWN))
		m_Buffer.push({ ActionBind::DOWN, InputBind::DOWN, wf });
	if (m_Mgs->input->getInputDown(InputBind::JUMP))
		m_Buffer.push({ ActionBind::JUMP, InputBind::JUMP, wf });
	if (m_Mgs->input->getInputDown(InputBind::ACT_X))
		m_Buffer.push({ ActionBind::ACT_X, InputBind::ACT_X, wf });
	if (m_Mgs->input->getInputDown(InputBind::ACT_Y))
		m_Buffer.push({ ActionBind::ACT_Y, InputBind::ACT_Y, wf });
}

void InputBuffer::clear()
{
	m_Buffer.clear();
}

void InputBuffer::drawBuffer()
{
	float size = m_Mgs->display->getLogWidth() / (float)(DEF_BUFFER_SIZE + 2);
	int wf = m_Mgs->time->getWorldFrame();

	for (int i = 0; i < count(); i++) {
		const InputEntry& entry = m_Buffer.peekAt(i);
		char ch = bindToChar(entry.logicalBind);
		if (ch == '\0') return;

		char txt[2] = { ch, '\0' };

		const float x = size * (i + 1);
		const float y = DEBUG_BUFF_Y;

		const float chX = (x + size / 2.0f) - (m_DebugFont.chSize * m_DebugFont.scale) / 2.0f;
		const float chY = (y + size / 2.0f) - ((m_DebugFont.chSize - m_DebugFont.baseline) * m_DebugFont.scale) / 2.0f;

		Sprite* spr = m_Mgs->sprite->get(RES::CIRCLE);
		m_Mgs->display->setSpriteModColor(spr, ColorRGBA::black());
		m_Mgs->display->drawSprite(RES::CIRCLE, { x, y }, { size, size });
		
		ColorRGBA clr = Colors::vividBlue;

		int diff = wf - entry.worldFrame;
		int dist = BUFFER_CLR_DECAY - diff;
		if (dist > 0) {
			clr.r = clamp(clr.r + dist, 0, 0xFF);
			clr.g = clamp(clr.g + dist, 0, 0xFF);
			clr.b = clamp(clr.b + dist, 0, 0xFF);
		}

		m_Mgs->display->setSpriteModColor(spr, clr);

		m_Mgs->display->drawSprite(RES::CIRCLE, { x + 3, y + 3 }, { size - 6, size - 6 });
		m_Mgs->display->drawString(RES::CH_32, { chX, chY }, txt, m_DebugFont);

		if (entry.physicalBind == InputBind::RIGHT || entry.physicalBind == InputBind::LEFT) {
			char smCh = (entry.physicalBind == InputBind::RIGHT) ? 'R' : 'L';
			char smTxt[2] = { smCh, '\0' };
			m_Mgs->display->drawString(RES::CH_16, { x + size * .65f, y + size * .65f }, smTxt, m_SmallDebugFont);
		}
		//m_Mgs->display->drawRect({ ch_x, ch_y }, { debFont.chSize * debFont.scale, debFont.chSize * debFont.scale }, ColorRGBA::red(), 2);
		m_Mgs->display->setSpriteModColor(spr, ColorRGBA::white());
	}
}

char InputBuffer::bindToChar(int a_Bind)
{
	switch (a_Bind) {
	case ActionBind::FORWARD: return 'F';
	case ActionBind::BACKWARD: return 'B';
	case ActionBind::UP: return 'U';
	case ActionBind::DOWN: return 'D';
	case ActionBind::JUMP: return 'J';
	case ActionBind::ACT_X: return 'X';
	case ActionBind::ACT_Y: return 'Y';
	default: return '\0';
	}
}