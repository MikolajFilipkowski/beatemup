#pragma once

#include "../../core/core.h"
#include "../game_structs.h"
#include "../game_loader.h"
#include "../config.h"
#include "../fonts.h"
#include "../input_buffer.h"
#include "../gameObjects/player.h"
#include "../gameObjects/obstacle.h"
#include "../gameObjects/player_camera.h"

class LevelScene : public GameScene {
private:
	GameState* m_GameState;
	GameLoader* m_GameLoader;
	GameSettings& m_Settings;
	LoadedLevel* m_LoadedLevel{};
	Player* m_Player{};
	PlayerCamera* m_Camera{};
	UIHealthbar* m_Healthbar{};
	Font m_ComboMulFont{ COMBO_MUL_FONT };
	Font m_ComboTxtFont{ COMBO_FONT };
	Font m_ComboHitsFont{ HITS_LIT_FONT };
	UISpriteContainer* m_Modal{nullptr};
	bool m_Paused{ false };
	int m_LevelQueued{ 0 };
	int m_EnemyCount{ 0 };
	bool m_PreserveScore{ false };

	void setFontsAlpha(float a_DiplayTimer);
	void reinitializeScene();
	LoadedZone* getZone(float a_PosX) const;
	LoadedZone* getWinZone() const;
public:
	LevelScene(Managers* a_Managers, GameState* a_GameState, GameLoader* a_GameLoader, GameSettings& a_Settings);
	void start() override;
	void update(float a_Dt) override;
	void fixedUpdate(float a_FixedDt) override;
	void draw() override;
	void drawComboMul(float a_DisplayTimer);
	void destroy() override;

	void saveScore() const;
	void changeLevel(int a_Id, bool PreserveScore = false);

	void createModal();
	void destroyModal();

	void pauseModal();
	void endModal(bool hasWon);
	void gameOverModal();
	bool loadFromFile(int a_Id);
};