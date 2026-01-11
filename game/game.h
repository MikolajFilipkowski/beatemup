#pragma once

#include "../core/core.h"
#include "config.h"
#include "gameObjects/player.h"
#include "scenes/level_scene.h"
#include "scenes/menu_scene.h"
#include "game_loader.h"

class Game : public Application {
protected:
	GameLoader m_GameLoader{};
	Font m_InfoFont;
public:
	Game();
	virtual ~Game() = default;
	bool onStart(Managers* a_Managers) override;
	void onUpdate(float a_Dt) override;
	void onFixedUpdate(float a_FixedDt) override;
	void onDraw() override;
	void onDestroy() override;

	void setupBindings();
	void loadCharsets();
};