#pragma once

#include "../core/core.h"

class Game : public Application {
public:
	Game() = default;
	virtual ~Game() = default;
	bool onStart(Managers* managers) override;
	void onUpdate(float dt) override;
	void onFixedUpdate(float fixed_dt) override;
	void onDraw() override;
	void onDestroy() override;
};