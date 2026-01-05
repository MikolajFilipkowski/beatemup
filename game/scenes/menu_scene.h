#pragma once

#include "../../core/core.h"
#include "../config.h"

class MenuScene : public GameScene {
protected:
	UIBackgroundContainer* menu;
	Camera* cam;
public:
	MenuScene(Managers* managers) : GameScene(managers), menu(nullptr), cam(nullptr) {}
	virtual ~MenuScene() { destroy(); }
	virtual void start() override;
	virtual void update(float dt) override;
	virtual void fixedUpdate(float fixed_dt) override;
	virtual void draw() override;
	virtual void destroy() override;
};