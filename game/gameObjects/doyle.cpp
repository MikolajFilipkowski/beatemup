#include "doyle.h"

#define DOYLE_ASSETS "game/assets/doyle/doyle_"

int Doyle::getAnimFromAct(int a_ActKey) const {
	switch (a_ActKey) {
	case Actions::IDLE:
		return RES::DOYLE_IDLE;
	case Actions::WALK:
		return RES::DOYLE_WALK;
	case Actions::HURT:
		return RES::DOYLE_HURT;
	case Actions::DEATH:
		return RES::DOYLE_DEATH;
	case Actions::DASH_FORWARD:
	case Actions::DASH_BACKWARD:
		return RES::DOYLE_DASH;
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
	float fabsX = fabsf(diffX);

	if (getCurrAction()->canMove) {
		if (fabsf(diffZ) > 5.0f && fabsX < 800.0f) {
			m_InputVel.z += (diffZ > 0) ? ENEMY_SPEED * 0.3f * Z_AXIS_MUL : -ENEMY_SPEED * 0.3f * Z_AXIS_MUL;
		}

		if (fabsX > 30.0f && fabsX < 900.0f) {
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

	if (inAttackRange(Actions::LIGHT_ATTACK)) {
		startAction(Actions::LIGHT_ATTACK);
		return;
	}

	int walkPrio = m_Mgs->object->getAction(Actions::WALK)->priority;
	if (currAct != nullptr && currAct->priority > walkPrio) return;

	if (isMoving) {
		startAction(Actions::WALK);
	}
	else {
		startAction(Actions::IDLE);
	}
}

Doyle::Doyle(Managers* a_Managers, GameState* a_GameState, Actor* a_Target, Transform a_Transform, int& a_EnemyCount)
	: Enemy(a_Managers, a_GameState, a_Target, a_Transform, a_EnemyCount)
{
	if (s_Instances == 0) {
		m_Mgs->sprite->load(DOYLE_ASSETS "idle.bmp", getAnimFromAct(Actions::IDLE));
		m_Mgs->sprite->load(DOYLE_ASSETS "walk.bmp", getAnimFromAct(Actions::WALK));
		m_Mgs->sprite->load(DOYLE_ASSETS "hurt.bmp", getAnimFromAct(Actions::HURT));
		m_Mgs->sprite->load(DOYLE_ASSETS "death.bmp", getAnimFromAct(Actions::DEATH));
		m_Mgs->sprite->load(DOYLE_ASSETS "dash.bmp", getAnimFromAct(Actions::DASH_FORWARD));
		m_Mgs->sprite->load(DOYLE_ASSETS "attack1.bmp", getAnimFromAct(Actions::LIGHT_ATTACK));
		m_Mgs->sprite->load(DOYLE_ASSETS "attack2.bmp", getAnimFromAct(Actions::HEAVY_ATTACK));
		m_Mgs->sprite->load(DOYLE_ASSETS "attack3.bmp", getAnimFromAct(Actions::WHEEL_PUNCH));

		loadAnims();
	}

	m_AttackChances.put(Actions::LIGHT_ATTACK, LIGHT_ATT_CHANCE);
	m_AttackChances.put(Actions::HEAVY_ATTACK, HEAVY_ATT_CHANCE);
	m_AttackChances.put(Actions::WHEEL_PUNCH, WHEEL_ATT_CHANCE);

	s_Instances++;
	m_EnemyCount++;
}

Doyle::~Doyle() {
	s_Instances--;
	m_EnemyCount--;

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

float Doyle::getAttackCooldown() const
{
	return ATT_COOLDOWN;
}
