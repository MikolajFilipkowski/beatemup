#pragma once

#include "../../core/core.h"
#include "../config.h"
#include "../input_buffer.h"

class LevelScene : public GameScene {
	InputBuffer* iBuffer;
public:
	LevelScene(Managers* managers, InputBuffer* buff) : GameScene(managers), iBuffer(buff) {}
	void start() override;
	void update(float dt) override;
	void fixedUpdate(float fixed_dt) override;
	void draw() override;
	void destroy() override;

	void loadFromFile(const char* fileName);
};