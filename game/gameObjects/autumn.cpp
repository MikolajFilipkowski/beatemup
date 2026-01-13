#include "autumn.h"

#define AUTUMN_ASSETS "game/assets/autumn/autumn_"

int Autumn::getAnimFromAct(int a_ActKey) const {
	switch (a_ActKey) {
	case Actions::IDLE:
		return RES::AUTUMN_IDLE;
	case Actions::WALK:
		return RES::AUTUMN_WALK;
	case Actions::JUMP:
		return RES::AUTUMN_JUMP;
	case Actions::LIGHT_ATTACK:
		return RES::AUTUMN_LIGHT_ATT;
	case Actions::HEAVY_ATTACK:
		return RES::AUTUMN_HEAVY_ATT;
	case Actions::WHEEL_PUNCH:
		return RES::AUTUMN_WHEEL_PUNCH;
	default:
		return RES::NONE;
	}
}

void Autumn::computeInput() {
}

Autumn::Autumn(Managers* a_Managers, Actor* a_Target, Transform a_Transform)
	: Enemy(a_Managers, a_Target, a_Transform)
{
	if (s_Instances == 0) {
		m_Mgs->sprite->load(AUTUMN_ASSETS "idle.bmp", getAnimFromAct(Actions::IDLE));
		m_Mgs->sprite->load(AUTUMN_ASSETS "walk.bmp", getAnimFromAct(Actions::WALK));
		m_Mgs->sprite->load(AUTUMN_ASSETS "jump.bmp", getAnimFromAct(Actions::JUMP));
		m_Mgs->sprite->load(AUTUMN_ASSETS "attack1.bmp", getAnimFromAct(Actions::LIGHT_ATTACK));
		m_Mgs->sprite->load(AUTUMN_ASSETS "attack2.bmp", getAnimFromAct(Actions::HEAVY_ATTACK));
		m_Mgs->sprite->load(AUTUMN_ASSETS "attack3.bmp", getAnimFromAct(Actions::WHEEL_PUNCH));

		loadAnims();
	}

	s_Instances++;
}

Autumn::~Autumn() {
	s_Instances--;

	if (s_Instances == 0) {
		unloadAnims();
	}
}

void Autumn::start()
{
	startAction(Actions::IDLE);
}

void Autumn::update(float a_Dt)
{
	m_Transform.pos = getIPos();
}
