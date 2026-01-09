#pragma once

#include "../core/core.h"
#include "config.h"
#include "gameObjects/player.h"
#include "scenes/level_scene.h"
#include "scenes/menu_scene.h"
#include "game_loader.h"

class Game : public Application {
protected:
	GameLoader* gmLoader;
	InputBuffer* iBuffer;
	Font infoFont;
	bool debugMode;
public:
	Game();
	virtual ~Game() = default;
	bool onStart(Managers* managers) override;
	void onUpdate(float dt) override;
	void onFixedUpdate(float fixed_dt) override;
	void onDraw() override;
	void onDestroy() override;

	bool inDebugMode() const;
	void setDebugMode(bool d);

	void setupBindings();
	void loadCharsets();
};