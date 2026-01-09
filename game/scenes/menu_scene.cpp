#include "menu_scene.h"

#include <cstring>
#include <cstdio>

#define MENU_ASSETS "game/assets/levels/menu/"
#define UI_ASSETS "game/assets/ui/"

constexpr float BTN_X_MUL = .35f;
constexpr float BTN_Y_MUL = .38f;
constexpr float BTN_W_MUL = .3f;

static void onLevelSelector(SDL_Event& ev, UIButton* button, Managers* mgs) {
	if (mgs->scene->getCurrentSceneIdx() == SceneID::MENU) {
		MenuScene* scene = (MenuScene*)mgs->scene->getCurrentScene();
		scene->setNextState(MenuState::LEVEL_SEL);
	}
};

static void onQuit(SDL_Event& ev, UIButton* button, Managers* mgs) {
	mgs->engine->stop();
};

static void onLevelChange(int level_id, SDL_Event& ev, UIButton* button, Managers* mgs) {
	mgs->scene->load(SceneID::LEVEL, false);
}

void MenuScene::getElInfo(const Dims& log_dims, float& el_w, float& el_h, float& dy)
{
	el_w = log_dims.width * BTN_X_MUL;
	el_h = log_dims.height * BTN_Y_MUL;
	dy = MENU_BTN_H + MENU_BTN_GAP;
}

MenuScene::MenuScene(Managers* managers) : GameScene(managers), menu(nullptr), cam(nullptr), nextState(MenuState::NONE) {
	title_font = {
		RES::CH_32,
		32,
		1.5f,
		.7f,
		ColorRGBA::white(),
		{ { 0x1E, 0x1E, 0x1E, 0xFF }, 6 },
		6
	};
	button_font = {
		RES::CH_32,
		32,
		1.25f,
		.75f,
		ColorRGBA::white(),
		Outline(),
		6
	};
	input_font = {
		RES::CH_16,
		16,
		1.75f,
		1.0f,
		{ 0x1E, 0x1E, 0x1E, 0xFF },
		Outline(),
		-2
	};
}

void MenuScene::start()
{
	const Dims log_dims = mgs->display->getLogDims();
	mgs->display->showCursor(true);

	loadUITextures();
	loadBackgrounds();
	loadMainMenu(log_dims);

	cam = new Camera(mgs);
	mgs->display->setActiveCamera(cam);
}

void MenuScene::update(float dt)
{
	if (nextState != MenuState::NONE) {
		changeState(nextState);
		return;
	}

	if (cam == nullptr) return;
	Vector3 pos = cam->getPosition();
	cam->setPosition({pos.x + .25f, pos.y, pos.z});
}

void MenuScene::fixedUpdate(float fixed_dt)
{
}

void MenuScene::draw()
{
	drawBackground();
}

void MenuScene::destroy()
{
	if (mgs->ui) mgs->ui->clear();

	mgs->sprite->unload(RES::MENU_BOXES);
	mgs->sprite->unload(RES::MENU_BUILDINGS);
	mgs->sprite->unload(RES::MENU_ROAD);
	mgs->sprite->unload(RES::MENU_SKY);
	mgs->sprite->unload(RES::MENU_WALL1);
	mgs->sprite->unload(RES::MENU_WALL2);
	mgs->sprite->unload(RES::MENU_WHEELS);
	mgs->sprite->unload(RES::PUNCH);
	mgs->sprite->unload(RES::UI_BIG_FRAME);
	mgs->sprite->unload(RES::UI_SMALL_FRAME);
	mgs->sprite->unload(RES::UI_BUTTON);
}

void MenuScene::changeState(MenuState state)
{
	const Dims log_dims = mgs->display->getLogDims();

	switch (nextState) {
	case MenuState::MAIN:
		loadMainMenu(log_dims);
		break;
	case MenuState::LEVEL_SEL:
		loadLevelSelector(log_dims);
		break;
	default:
		break;
	}

	nextState = MenuState::NONE;
}

void MenuScene::setNextState(MenuState state)
{
	nextState = state;
}

void MenuScene::createMenuCont(const Dims& log_dims)
{
	menu = new UISpriteBackgroundContainer(
		mgs,
		{ log_dims.width * .23f, log_dims.height * .18f },
		{ log_dims.width * .54f, log_dims.height * .64f }
	);
	menu->setSprite(RES::UI_BIG_FRAME);
}

void MenuScene::loadBackgrounds()
{
	mgs->sprite->load(MENU_ASSETS "boxes.bmp", RES::MENU_BOXES);
	mgs->sprite->load(MENU_ASSETS "buildings.bmp", RES::MENU_BUILDINGS);
	mgs->sprite->load(MENU_ASSETS "road.bmp", RES::MENU_ROAD);
	mgs->sprite->load(MENU_ASSETS "sky.bmp", RES::MENU_SKY);
	mgs->sprite->load(MENU_ASSETS "wall1.bmp", RES::MENU_WALL1);
	mgs->sprite->load(MENU_ASSETS "wall2.bmp", RES::MENU_WALL2);
	mgs->sprite->load(MENU_ASSETS "wheels.bmp", RES::MENU_WHEELS);

	addLayer(RES::MENU_SKY, 0.1f, 1280.0f, 720.0f);
	addLayer(RES::MENU_BUILDINGS, 0.2f, 1280.0f, 720.0f);
	addLayer(RES::MENU_WALL2, 1.0f, 1280.0f, 720.0f);
	addLayer(RES::MENU_WALL1, 1.0f, 1280.0f, 720.0f);
	addLayer(RES::MENU_BOXES, 1.0f, 1280.0f, 720.0f);
	addLayer(RES::MENU_WHEELS, 1.0f, 1280.0f, 720.0f);
	addLayer(RES::MENU_ROAD, 1.0f, 2000.0f, 720.f);
}

void MenuScene::loadUITextures()
{
	mgs->sprite->load(UI_ASSETS "big_frame.bmp", RES::UI_BIG_FRAME);
	mgs->sprite->load(UI_ASSETS "small_frame.bmp", RES::UI_SMALL_FRAME);
	mgs->sprite->load(UI_ASSETS "button.bmp", RES::UI_BUTTON);
	mgs->sprite->load(UI_ASSETS "text_input.bmp", RES::UI_TEXT_INPUT);
}

void MenuScene::loadTitle(const Dims& log_dims)
{
	mgs->sprite->load(UI_ASSETS "punch.bmp", RES::PUNCH);
	mgs->sprite->setSpriteColor(RES::PUNCH, title_font.color);

	const float ch_size = title_font.chSize * title_font.scale;
	const char* t_text = "Beat em up";
	const float t_size = (strlen(t_text)) * ch_size * title_font.spacing;

	const float t_x = (log_dims.width / 2.0f) - (t_size / 2.0f);
	const float t_y = log_dims.height * .225f;

	UISpriteBackgroundContainer* punch = new UISpriteBackgroundContainer(
		mgs, { t_x + IC_OFF_X, t_y + IC_OFF_Y }, { IC_SIZE, IC_SIZE }
	);
	punch->setSprite(RES::PUNCH);

	UITextElement* title = new UITextElement(
		mgs,
		{ t_x, t_y },
		{ t_size, ch_size },
		title_font
	);

	title->setText(t_text);
	menu->addElement((UIElement*)punch);
	menu->addElement((UIElement*)title);
}

void MenuScene::loadMainMenu(const Dims& log_dims)
{
	createMenuCont(log_dims);
	loadTitle(log_dims);

	float el_w, el_h, dy;
	getElInfo(log_dims, el_w, el_h, dy);

	UITextInput* txt_input = new UITextInput(
		mgs,
		{ log_dims.width * .35f, log_dims.height * .32f },
		{ log_dims.width * .3f, MENU_BTN_H * 1.5f },
		input_font,
		10,
		{ 50, 0 }
	);
	//txt_input->setBackground(ColorRGBA::white());
	//txt_input->setBorder(ColorRGBA::black(), 3);
	txt_input->setSprite(RES::UI_TEXT_INPUT);
	txt_input->setPlaceholder("Pseudonim", { 0x77, 0x77, 0x77, 0xFF });

	menu->addElement((UIElement*)txt_input);

	loadMainButtons(el_w, el_h, dy, log_dims);

	mgs->ui->add((UIElement*)menu);
}

void MenuScene::loadLevelSelector(const Dims& log_dims)
{
	mgs->ui->remove(menu);
	
	createMenuCont(log_dims);

	mgs->ui->add(menu);

	const ColorRGBA t_color = { 0x1E, 0x1E, 0x1E, 0xFF };

	const Font t_font = {
		RES::CH_32,
		32,
		1.5f,
		.75f,
		t_color
	};

	loadTitle(log_dims);

	float el_w, el_h, dy;
	getElInfo(log_dims, el_w, el_h, dy);

	loadLevelButtons(el_w, el_h, dy, log_dims);
}

void MenuScene::loadMainButtons(float el_w, float el_h, float dy, const Dims& log_dims)
{
	UIButton* btn_lvl = new UIButton(
		mgs,
		{ el_w, el_h + dy },
		{ log_dims.width * .3f, MENU_BTN_H },
		button_font,
		onLevelSelector
	);
	btn_lvl->setSprite(RES::UI_BUTTON);
	btn_lvl->setText("Graj");

	UIButton* btn_scores = new UIButton(
		mgs,
		{ el_w, el_h + 2 * dy },
		{ log_dims.width * .3f, MENU_BTN_H },
		button_font
	);
	btn_scores->setSprite(RES::UI_BUTTON);
	btn_scores->setText("Wyniki");

	UIButton* btn_quit = new UIButton(
		mgs,
		{ el_w, el_h + 3 * dy },
		{ log_dims.width * .3f, MENU_BTN_H },
		button_font,
		onQuit
	);
	btn_quit->setSprite(RES::UI_BUTTON);
	btn_quit->setText("Wyjscie");

	menu->addElement((UIElement*)btn_lvl);
	menu->addElement((UIElement*)btn_scores);
	menu->addElement((UIElement*)btn_quit);
}

void MenuScene::loadLevelButtons(float el_w, float el_h, float dy, const Dims& log_dims)
{
	auto onLvl1 = [](SDL_Event& ev, UIButton* button, Managers* mgs) { onLevelChange(1, ev, button, mgs); };
	auto onLvl2 = [](SDL_Event& ev, UIButton* button, Managers* mgs) { onLevelChange(2, ev, button, mgs); };

	UIButton* btn_lvl1 = new UIButton(
		mgs,
		{ el_w, el_h + dy },
		{ log_dims.width * .3f, MENU_BTN_H },
		button_font,
		onLvl1
	);
	btn_lvl1->setSprite(RES::UI_BUTTON);
	btn_lvl1->setText("Etap 1");

	UIButton* btn_lvl2 = new UIButton(
		mgs,
		{ el_w, el_h + 2 * dy },
		{ log_dims.width * .3f, MENU_BTN_H },
		button_font,
		onLvl2
	);
	btn_lvl2->setSprite(RES::UI_BUTTON);
	btn_lvl2->setText("Etap 2");

	menu->addElement((UIElement*)btn_lvl1);
	menu->addElement((UIElement*)btn_lvl2);
}