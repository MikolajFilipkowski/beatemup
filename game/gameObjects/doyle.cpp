#include "doyle.h"

#define DOYLE_ASSETS "game/assets/doyle/doyle_"

int Doyle::getAnimFromAct(int a_ActKey) const {
	switch (a_ActKey) {
	case Actions::IDLE:
		return RES::DOYLE_IDLE;
	case Actions::WALK:
		return RES::DOYLE_WALK;
	case Actions::JUMP:
		return RES::DOYLE_JUMP;
	case Actions::LIGHT_ATTACK:
		return RES::DOYLE_LIGHT_ATT;
	case Actions::HEAVY_ATTACK:
		return RES::DOYLE_HEAVY_ATT;
	case Actions::WHEEL_PUNCH:
		return RES::DOYLE_WHEEL_PUNCH;
	default:
		return RES::NONE;
	}
}

void Doyle::computeInput() {

}

Doyle::Doyle(Managers* a_Managers, Actor* a_Target, Transform a_Transform)
	: Enemy(a_Managers, a_Target, a_Transform)
{
	if (s_Instances == 0) {
		m_Mgs->sprite->load(DOYLE_ASSETS "idle.bmp", getAnimFromAct(Actions::IDLE));
		m_Mgs->sprite->load(DOYLE_ASSETS "walk.bmp", getAnimFromAct(Actions::WALK));
		m_Mgs->sprite->load(DOYLE_ASSETS "jump.bmp", getAnimFromAct(Actions::JUMP));
		m_Mgs->sprite->load(DOYLE_ASSETS "attack1.bmp", getAnimFromAct(Actions::LIGHT_ATTACK));
		m_Mgs->sprite->load(DOYLE_ASSETS "attack2.bmp", getAnimFromAct(Actions::HEAVY_ATTACK));
		m_Mgs->sprite->load(DOYLE_ASSETS "attack3.bmp", getAnimFromAct(Actions::WHEEL_PUNCH));

		loadAnims();
	}
	
	s_Instances++;
}

Doyle::~Doyle() {
	s_Instances--;

	if (s_Instances == 0) {
		unloadAnims();
	}
}

void Doyle::start() {
	startAction(Actions::IDLE);
}

void Doyle::update(float a_Dt)
{
	m_Transform.pos = getIPos();
}
