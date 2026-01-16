#pragma once

#include "../core/core.h"
#include "config.h"
#include "gameObjects/player.h"
#include "scenes/level_scene.h"
#include "scenes/menu_scene.h"
#include "game_loader.h"
#include "game_structs.h"

class Game : public Application {
protected:
	GameLoader& m_GameLoader;
	GameSettings& m_Settings;
	GameState m_GameState;
public:
	Game(GameLoader& a_GameLoader, GameSettings& a_Settings);
	virtual ~Game() = default;
	bool onStart(Managers* a_Managers) override;
	void onUpdate(float a_Dt) override;
	void onFixedUpdate(float a_FixedDt) override;
	void onDraw() override;
	void onDestroy() override;

	void loadUITextures();
	void setupBindings();
	void loadCharsets();
	void setupStatuses();
};