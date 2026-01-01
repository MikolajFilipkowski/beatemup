#pragma once

#include "../core/core.h"
#include "gameObjects/player.h"

class Game : public Application {
private:
	Player* player;
public:
	Game();
	virtual ~Game() = default;
	bool onStart(Managers* managers) override;
	void onUpdate(float dt) override;
	void onFixedUpdate(float fixed_dt) override;
	void onDraw() override;
	void onDestroy() override;
};