#pragma once

#include "../../core/core.h"
#include "../game_structs.h"
#include "../config.h"
#include "../fonts.h"
#include "../input_buffer.h"
#include "../gameObjects/player.h"
#include "../gameObjects/player_camera.h"

class LevelScene : public GameScene {
private:
	GameState* m_GameState;
	Player* m_Player{};
	PlayerCamera* m_Camera{};
	UIHealthbar* m_Healthbar{};
	Font m_ComboMulFont{ COMBO_MUL_FONT };
	Font m_ComboTxtFont{ COMBO_FONT };
	Font m_ComboHitsFont{ HITS_LIT_FONT };
	UISpriteContainer* m_Modal{nullptr};
	bool m_Paused{ false };

	void setFontsAlpha(float a_DiplayTimer);
public:
	LevelScene(Managers* a_Managers, GameState* a_GameState);
	void start() override;
	void update(float a_Dt) override;
	void fixedUpdate(float a_FixedDt) override;
	void draw() override;
	void drawComboMul(float a_DisplayTimer);
	void destroy() override;

	void createModal();
	void destroyModal();

	void pauseModal();
	void endModal(bool hasWon);
	void gameOverModal();
	void loadFromFile(const char* a_FileName);
};