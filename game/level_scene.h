#pragma once

#include "../core/core.h"
#include "config.h"

class LevelScene : public GameScene {
public:
	LevelScene(Managers* managers) : GameScene(managers) {}
	void start() override;
	void update(float dt) override;
	void fixedUpdate(float fixed_dt) override;
	void draw() override;
	void destroy() override;

	void loadFromFile(const char* fileName);
};