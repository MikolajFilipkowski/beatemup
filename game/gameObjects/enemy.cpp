#include "enemy.h"

Enemy::Enemy(Managers* a_Managers, GameState* a_GameState, Actor* a_Target, Transform a_Transform, int& a_EnemyCount)
	: Actor(a_Managers, a_GameState, a_Transform), m_Target(a_Target), m_EnemyCount(a_EnemyCount) {}

Enemy::~Enemy() {}

bool Enemy::chooseAttack()
{
	if (m_AttackTimer > 0.0f || m_StunTimer > 0.0f) return false;
	int rnd = rand() % totalChance();

	auto& acts = m_Mgs->object->getAllActions();

	int totalCalculated = 0;

	for (auto& pair : acts) {
		ActionData* act = pair.value;
		if (!(act->owner & getType() && act->isAttack()))
			continue;

		bool canUse = inAttackRange(pair.key);
		totalCalculated += m_AttackChances.get(pair.key);

		if (rnd < totalCalculated) {
			if (canUse) {
				startAction(pair.key);
				m_AttackTimer = pair.value->getTotalDuration() + getAttackCooldown();
				return true;
			}

			m_AttackTimer = getAttackCooldown();
			return false;
		}
	}

	return false;
}

void Enemy::update(float a_Dt)
{
}

void Enemy::die()
{
	m_Mgs->object->remove(this);
}

Uint8 Enemy::getType() const
{
	return ObjectType::ENEMY;
}

int Enemy::totalChance() const
{
	int sum = 0;
	for (auto& pair : m_AttackChances) {
		sum += pair.value;
	}
	return sum;
}

bool Enemy::inAttackRange(int a_ActKey) const
{
	if (m_Target->isDying()) 
		return false;
	auto act = m_Mgs->object->getAction(a_ActKey);

	Vector2 attRange = act->getAttackRange();
	Rect targetHurt = m_Target->getCurrFrame().hurtbox;
	Vector3& targetPos = m_Target->getRb().currPos;

	float currDistZ = fabsf(m_Rb.currPos.z - targetPos.z);
	if (currDistZ > MAX_DZ_HIT) return false;

	float attDistX = attRange.x + (targetHurt.x * AI_ATT_MARGIN);

	float currDistX = fabsf(m_Rb.currPos.x - targetPos.x);
	float currDistY = fabsf(m_Rb.currPos.y - targetPos.y);

	return currDistX <= attDistX && currDistY <= attRange.y;
}

float Enemy::getAttackCooldown() const
{
	return 0.0f;
}
