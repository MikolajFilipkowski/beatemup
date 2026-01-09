#pragma once

#include "../../core/core.h"
#include "../config.h"
#include "../input_buffer.h"

class Player : public AnimatableObject {
protected:
	InputBuffer* iBuffer;
	float hp;
	bool jumpRequested;
	bool isGrounded;
	bool isAttacking;
	float attackTimer;
public:
	Player(Managers* mgs, InputBuffer* buff, Transform tr = Transform::zero());
	virtual ~Player() override;
	virtual void start() override;
	virtual void update(float dt) override;
	virtual void fixedUpdate(float fixed_dt) override;
	virtual void draw() override;
};