#pragma once

#include "../../core/core.h"

class Player;

class PlayerCamera : public Camera {
protected:
	Player* m_Player;
public:
	PlayerCamera(Managers* a_Managers, Player* a_Ply, Vector3 a_Pos = { 0, 0, 500.0f });
	~PlayerCamera();

	void update(float a_Dt) override;
	void fixedUpdate(float a_FixedDt) override;
};