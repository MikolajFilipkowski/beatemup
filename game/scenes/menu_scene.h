#pragma once

#include "../../core/core.h"
#include "../config.h"

enum class MenuState {
	NONE,
	MAIN,
	LEVEL_SEL,
};

class MenuScene : public GameScene {
protected:
	UISpriteBackgroundContainer* m_Menu{};
	Camera* m_Cam{};
	MenuState m_NextState{ MenuState::NONE };
	Font m_TitleFont;
	Font m_ButtonFont;
	Font m_InputFont;

	void getElInfo(const Dims& a_LogDims, float& a_ElW, float& a_ElH, float& a_Dy);
public:
	MenuScene(Managers* a_Managers);
	virtual ~MenuScene() { destroy(); }
	virtual void start() override;
	virtual void update(float a_Dt) override;
	virtual void fixedUpdate(float a_FixedDt) override;
	virtual void draw() override;
	virtual void destroy() override;

	void changeState(MenuState a_State);
	void setNextState(MenuState a_State);

	void createMenuCont(const Dims& a_LogDims);

	void loadBackgrounds();
	void loadUITextures();
	void loadTitle(const Dims& a_LogDims);
	void loadMainMenu(const Dims& a_LogDims);
	void loadLevelSelector(const Dims& a_LogDims);

	void loadMainButtons(float a_ElW, float a_ElH, float a_Dy, const Dims& a_LogDims);
	void loadLevelButtons(float a_ElW, float a_ElH, float a_Dy, const Dims& a_LogDims);
};