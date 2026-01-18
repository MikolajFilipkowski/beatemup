#include "obstacle.h"

Obstacle::Obstacle(Managers* a_Managers, Transform a_Transform, Vector3 a_Dims, int a_SpriteKey, float a_Mass)
	: SpriteObject(a_Managers, a_Transform, a_SpriteKey), m_Dims(a_Dims)
{
	m_Rb.mass = a_Mass;
}

Obstacle::~Obstacle()
{
}

Cuboid Obstacle::getCollBox()
{
	return {
		m_Rb.currPos.x - m_Dims.x / 2.0f,
		m_Rb.currPos.y - m_Dims.y / 2.0f,
		m_Rb.currPos.z,
		m_Dims.x,
		m_Dims.y,
		m_Dims.z
	};
}
