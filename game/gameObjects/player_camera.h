#pragma once

#include "../../core/core.h"

class Player;

class PlayerCamera : public Camera {
protected:
	Player* ply;
public:
	PlayerCamera(Managers* mgs, Player* ply, Vector3 pos = { 0, 0, 500.0f });
	~PlayerCamera();

	void update(float dt) override;
	void fixedUpdate(float fixed_dt) override;
};