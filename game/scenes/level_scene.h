#pragma once

#include "../../core/core.h"
#include "../config.h"
#include "../input_buffer.h"
#include "../gameObjects/player.h"
#include "../gameObjects/player_camera.h"

class LevelScene : public GameScene {
private:
	Player* m_Player{};
	PlayerCamera* m_Camera{};
	UIHealthbar* m_Healthbar{};
public:
	LevelScene(Managers* a_Managers) : GameScene(a_Managers) {}
	void start() override;
	void update(float a_Dt) override;
	void fixedUpdate(float a_FixedDt) override;
	void draw() override;
	void destroy() override;

	void loadFromFile(const char* a_FileName);
};