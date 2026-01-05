#pragma once

#include "../../core/core.h"
#include "../config.h"

class Player : public AnimatableObject {
protected:
	bool jumpRequested;
	bool isGrounded;
	bool isAttacking;
	float attackTimer;
public:
	Player(Managers* mgs, Transform tr = Transform::zero());
	virtual ~Player() override;
	virtual void start() override;
	virtual void update(float dt) override;
	virtual void fixedUpdate(float fixed_dt) override;
	virtual void draw() override;
};