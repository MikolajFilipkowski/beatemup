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
	UISpriteBackgroundContainer* menu;
	Camera* cam;
	MenuState nextState;
	Font title_font;
	Font button_font;
	Font input_font;

	void getElInfo(const Dims& log_dims, float& el_w, float& el_h, float& dy);
public:
	MenuScene(Managers* managers);
	virtual ~MenuScene() { destroy(); }
	virtual void start() override;
	virtual void update(float dt) override;
	virtual void fixedUpdate(float fixed_dt) override;
	virtual void draw() override;
	virtual void destroy() override;

	void changeState(MenuState state);
	void setNextState(MenuState state);

	void createMenuCont(const Dims& log_dims);

	void loadBackgrounds();
	void loadUITextures();
	void loadTitle(const Dims& log_dims);
	void loadMainMenu(const Dims& log_dims);
	void loadLevelSelector(const Dims& log_dims);

	void loadMainButtons(float el_w, float el_y, float dy, const Dims& log_dims);
	void loadLevelButtons(float el_w, float el_y, float dy, const Dims& log_dims);
};