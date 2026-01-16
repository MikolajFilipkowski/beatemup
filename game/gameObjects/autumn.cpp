#include "autumn.h"

#include <cstdio>

#define AUTUMN_ASSETS "game/assets/autumn/autumn_"

int Autumn::getAnimFromAct(int a_ActKey) const {
	switch (a_ActKey) {
	case Actions::IDLE:
		return RES::AUTUMN_IDLE;
	case Actions::WALK:
		return RES::AUTUMN_WALK;
	case Actions::HURT:
		return RES::AUTUMN_HURT;
	case Actions::DEATH:
		return RES::AUTUMN_DEATH;
	case Actions::DASH_FORWARD:
	case Actions::DASH_BACKWARD:
		return RES::AUTUMN_DASH;
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
	if (m_AttackTimer > 0.0f) {
		m_AttackTimer -= m_Mgs->time->getFixedDt();
		m_AttackTimer = (m_AttackTimer <= 0.0f) ? 0.0f : m_AttackTimer;
	}

	if (!m_Target || !m_Grounded) return;

	ActionData* currAct = getCurrAction();
	bool canInterrupt = (currAct == nullptr || currAct->interruptible);
	bool isMoving = false;

	Vector3 tPos = m_Target->getRb().currPos;
	Vector3& ePos = m_Rb.currPos;

	int mul = (m_Id % AI_Z_LAYERS) - (AI_Z_LAYERS / 2);
	tPos.z += AI_Z_SEPARATION * mul;

	float diffX = tPos.x - ePos.x;
	float diffZ = tPos.z - ePos.z;

	if (getCurrAction()->canMove) {
		if (fabsf(diffZ) > 5.0f) {
			m_InputVel.z += (diffZ > 0) ? ENEMY_SPEED * Z_AXIS_MUL : -ENEMY_SPEED * Z_AXIS_MUL;
		}

		if (fabsf(diffX) > 30.0f) {
			m_InputVel.x += (diffX > 0) ? ENEMY_SPEED : -ENEMY_SPEED;
		}

		if (m_InputVel.x != 0) {
			m_Transform.flip = (m_InputVel.x > 0) ? NO_FLIP : H_FLIP;
			isMoving = true;
		}

		if (m_InputVel.z != 0)
			isMoving = true;
	}

	if (!canInterrupt) return;

	bool attacked = chooseAttack();
	if (attacked) return;

	int walkPrio = m_Mgs->object->getAction(Actions::WALK)->priority;
	if (currAct != nullptr && currAct->priority > walkPrio) return;

	if (isMoving) {
		startAction(Actions::WALK);
	}
	else {
		startAction(Actions::IDLE);
	}
}



Autumn::Autumn(Managers* a_Managers, GameState* a_GameState, Actor* a_Target, Transform a_Transform)
	: Enemy(a_Managers, a_GameState, a_Target, a_Transform)
{
	if (s_Instances == 0) {
		m_Mgs->sprite->load(AUTUMN_ASSETS "idle.bmp", getAnimFromAct(Actions::IDLE));
		m_Mgs->sprite->load(AUTUMN_ASSETS "walk.bmp", getAnimFromAct(Actions::WALK));
		m_Mgs->sprite->load(AUTUMN_ASSETS "hurt.bmp", getAnimFromAct(Actions::HURT));
		m_Mgs->sprite->load(AUTUMN_ASSETS "death.bmp", getAnimFromAct(Actions::DEATH));
		m_Mgs->sprite->load(AUTUMN_ASSETS "dash.bmp", getAnimFromAct(Actions::DASH_FORWARD));
		m_Mgs->sprite->load(AUTUMN_ASSETS "attack1.bmp", getAnimFromAct(Actions::LIGHT_ATTACK));
		m_Mgs->sprite->load(AUTUMN_ASSETS "attack2.bmp", getAnimFromAct(Actions::HEAVY_ATTACK));
		m_Mgs->sprite->load(AUTUMN_ASSETS "attack3.bmp", getAnimFromAct(Actions::WHEEL_PUNCH));

		loadAnims();
	}

	m_AttackChances.put(Actions::LIGHT_ATTACK, LIGHT_ATT_CHANCE);
	m_AttackChances.put(Actions::HEAVY_ATTACK, HEAVY_ATT_CHANCE);
	m_AttackChances.put(Actions::WHEEL_PUNCH, WHEEL_ATT_CHANCE);

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

float Autumn::getAttackCooldown() const
{
	return ATT_COOLDOWN;
}
