#pragma once

#include "../../core/core.h"
#include "../config.h"

namespace PlayerActions {
	
}

class Player : public AnimatableObject {
protected:
	bool jumpRequested;
	bool isGrounded;
	Camera* plyCam;
public:
	Player(Managers* mgs, Camera* cam, Transform tr = Transform::zero());
	virtual ~Player() override;
	virtual void start() override;
	virtual void update(float dt) override;
	virtual void fixedUpdate(float fixed_dt) override;
	virtual void draw() override;
};