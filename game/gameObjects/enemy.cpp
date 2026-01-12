#include "enemy.h"

Enemy::Enemy(Managers* a_Managers, Player* a_Player, Transform a_Transform)
	: Actor(a_Managers, a_Transform), m_Player(a_Player)
{

}

Enemy::~Enemy()
{
}

void Enemy::start()
{
}

void Enemy::update(float a_Dt)
{
}

Uint8 Enemy::getType() const
{
	return ObjectType::ENEMY;
}
