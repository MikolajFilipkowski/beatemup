#pragma once

#include "../../core/core.h"
#include "../config.h"
#include "../game_structs.h"

class Obstacle : SpriteObject {
protected:
	Vector3 m_Dims;
public:
	Obstacle(Managers* a_Managers, Transform a_Transform, Vector3 a_Dims, int a_SpriteKey, float a_Mass);
	virtual ~Obstacle() override;

	virtual Cuboid getCollBox() override;
};