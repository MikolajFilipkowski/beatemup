#include "enemy.h"

Enemy::Enemy(Managers* a_Managers, Actor* a_Target, Transform a_Transform)
	: Actor(a_Managers, a_Transform), m_Target(a_Target)
{
	
}

Enemy::~Enemy() {}

void Enemy::update(float a_Dt)
{
}

Uint8 Enemy::getType() const
{
	return ObjectType::ENEMY;
}