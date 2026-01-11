#include "menu_scene.h"

#include <cstring>
#include <cstdio>

#define MENU_ASSETS "game/assets/levels/menu/"
#define UI_ASSETS "game/assets/ui/"

constexpr float BTN_X_MUL = .35f;
constexpr float BTN_Y_MUL = .38f;
constexpr float BTN_W_MUL = .3f;

static void onLevelSelector(SDL_Event& ev, UIButton* button, Managers* m_Mgs) {
	if (m_Mgs->scene->getCurrentSceneIdx() == SceneID::MENU) {
		MenuScene* scene = (MenuScene*)m_Mgs->scene->getCurrentScene();
		scene->setNextState(MenuState::LEVEL_SEL);
	}
};

static void onQuit(SDL_Event& ev, UIButton* button, Managers* m_Mgs) {
	m_Mgs->engine->stop();
};

static void onLevelChange(int level_id, SDL_Event& ev, UIButton* button, Managers* m_Mgs) {
	m_Mgs->scene->load(SceneID::LEVEL, false);
}

void MenuScene::getElInfo(const Dims& log_dims, float& el_w, float& el_h, float& dy)
{
	el_w = log_dims.width * BTN_X_MUL;
	el_h = log_dims.height * BTN_Y_MUL;
	dy = MENU_BTN_H + MENU_BTN_GAP;
}

MenuScene::MenuScene(Managers* a_Managers) : GameScene(a_Managers) {
	m_TitleFont = {
		RES::CH_32,
		32,
		1.5f,
		.7f,
		ColorRGBA::white(),
		{ { 0x1E, 0x1E, 0x1E, 0xFF }, 6 },
		6
	};
	m_ButtonFont = {
		RES::CH_32,
		32,
		1.25f,
		.75f,
		ColorRGBA::white(),
		Outline(),
		6
	};
	m_InputFont = {
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
	const Dims log_dims = m_Mgs->display->getLogDims();
	m_Mgs->display->showCursor(true);

	loadUITextures();
	loadBackgrounds();
	loadMainMenu(log_dims);

	m_Cam = new Camera(m_Mgs);
	m_Mgs->display->setActiveCamera(m_Cam);
}

void MenuScene::update(float a_Dt)
{
	if (m_NextState != MenuState::NONE) {
		changeState(m_NextState);
		return;
	}

	if (m_Cam == nullptr) return;
	Vector3 pos = m_Cam->getPosition();
	m_Cam->setPosition({pos.x + .25f, pos.y, pos.z});
}

void MenuScene::fixedUpdate(float a_FixedDt)
{
}

void MenuScene::draw()
{
	drawBackground();
}

void MenuScene::destroy()
{
	if (m_Mgs->ui) m_Mgs->ui->clear();

	m_Mgs->sprite->unload(RES::MENU_BOXES);
	m_Mgs->sprite->unload(RES::MENU_BUILDINGS);
	m_Mgs->sprite->unload(RES::MENU_ROAD);
	m_Mgs->sprite->unload(RES::MENU_SKY);
	m_Mgs->sprite->unload(RES::MENU_WALL1);
	m_Mgs->sprite->unload(RES::MENU_WALL2);
	m_Mgs->sprite->unload(RES::MENU_WHEELS);
	m_Mgs->sprite->unload(RES::PUNCH);
	m_Mgs->sprite->unload(RES::UI_BIG_FRAME);
	m_Mgs->sprite->unload(RES::UI_SMALL_FRAME);
	m_Mgs->sprite->unload(RES::UI_BUTTON);
}

void MenuScene::changeState(MenuState a_State)
{
	const Dims log_dims = m_Mgs->display->getLogDims();

	switch (m_NextState) {
	case MenuState::MAIN:
		loadMainMenu(log_dims);
		break;
	case MenuState::LEVEL_SEL:
		loadLevelSelector(log_dims);
		break;
	default:
		break;
	}

	m_NextState = MenuState::NONE;
}

void MenuScene::setNextState(MenuState a_State)
{
	m_NextState = a_State;
}

void MenuScene::createMenuCont(const Dims& a_LogDims)
{
	m_Menu = new UISpriteBackgroundContainer(
		m_Mgs,
		{ a_LogDims.width * .23f, a_LogDims.height * .18f },
		{ a_LogDims.width * .54f, a_LogDims.height * .64f }
	);
	m_Menu->setSprite(RES::UI_BIG_FRAME);
}

void MenuScene::loadBackgrounds()
{
	m_Mgs->sprite->load(MENU_ASSETS "boxes.bmp", RES::MENU_BOXES);
	m_Mgs->sprite->load(MENU_ASSETS "buildings.bmp", RES::MENU_BUILDINGS);
	m_Mgs->sprite->load(MENU_ASSETS "road.bmp", RES::MENU_ROAD);
	m_Mgs->sprite->load(MENU_ASSETS "sky.bmp", RES::MENU_SKY);
	m_Mgs->sprite->load(MENU_ASSETS "wall1.bmp", RES::MENU_WALL1);
	m_Mgs->sprite->load(MENU_ASSETS "wall2.bmp", RES::MENU_WALL2);
	m_Mgs->sprite->load(MENU_ASSETS "wheels.bmp", RES::MENU_WHEELS);

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
	m_Mgs->sprite->load(UI_ASSETS "big_frame.bmp", RES::UI_BIG_FRAME);
	m_Mgs->sprite->load(UI_ASSETS "small_frame.bmp", RES::UI_SMALL_FRAME);
	m_Mgs->sprite->load(UI_ASSETS "button.bmp", RES::UI_BUTTON);
	m_Mgs->sprite->load(UI_ASSETS "text_input.bmp", RES::UI_TEXT_INPUT);
}

void MenuScene::loadTitle(const Dims& a_LogDims)
{
	m_Mgs->sprite->load(UI_ASSETS "punch.bmp", RES::PUNCH);
	m_Mgs->sprite->setSpriteColor(RES::PUNCH, m_TitleFont.color);

	const float ch_size = m_TitleFont.chSize * m_TitleFont.scale;
	const char* t_text = "Beat em up";
	const float t_size = (strlen(t_text)) * ch_size * m_TitleFont.spacing;

	const float t_x = (a_LogDims.width / 2.0f) - (t_size / 2.0f);
	const float t_y = a_LogDims.height * .225f;

	UISpriteBackgroundContainer* punch = new UISpriteBackgroundContainer(
		m_Mgs, { t_x + IC_OFF_X, t_y + IC_OFF_Y }, { IC_SIZE, IC_SIZE }
	);
	punch->setSprite(RES::PUNCH);

	UITextElement* title = new UITextElement(
		m_Mgs,
		{ t_x, t_y },
		{ t_size, ch_size },
		m_TitleFont
	);

	title->setText(t_text);
	m_Menu->addElement((UIElement*)punch);
	m_Menu->addElement((UIElement*)title);
}

void MenuScene::loadMainMenu(const Dims& a_LogDims)
{
	createMenuCont(a_LogDims);
	loadTitle(a_LogDims);

	float el_w, el_h, dy;
	getElInfo(a_LogDims, el_w, el_h, dy);

	UITextInput* txt_input = new UITextInput(
		m_Mgs,
		{ a_LogDims.width * .35f, a_LogDims.height * .32f },
		{ a_LogDims.width * .3f, MENU_BTN_H * 1.5f },
		m_InputFont,
		10,
		{ 50, 0 }
	);
	//txt_input->setBackground(ColorRGBA::white());
	//txt_input->setBorder(ColorRGBA::black(), 3);
	txt_input->setSprite(RES::UI_TEXT_INPUT);
	txt_input->setPlaceholder("Pseudonim", { 0x77, 0x77, 0x77, 0xFF });

	m_Menu->addElement((UIElement*)txt_input);

	loadMainButtons(el_w, el_h, dy, a_LogDims);

	m_Mgs->ui->add((UIElement*)m_Menu);
}

void MenuScene::loadLevelSelector(const Dims& a_LogDims)
{
	m_Mgs->ui->remove(m_Menu);
	
	createMenuCont(a_LogDims);

	m_Mgs->ui->add(m_Menu);

	const ColorRGBA t_color = { 0x1E, 0x1E, 0x1E, 0xFF };

	const Font t_font = {
		RES::CH_32,
		32,
		1.5f,
		.75f,
		t_color
	};

	loadTitle(a_LogDims);

	float el_w, el_h, dy;
	getElInfo(a_LogDims, el_w, el_h, dy);

	loadLevelButtons(el_w, el_h, dy, a_LogDims);
}

void MenuScene::loadMainButtons(float a_ElW, float a_ElH, float a_Dy, const Dims& a_LogDims)
{
	UIButton* btn_lvl = new UIButton(
		m_Mgs,
		{ a_ElW, a_ElH + a_Dy },
		{ a_LogDims.width * .3f, MENU_BTN_H },
		m_ButtonFont,
		onLevelSelector
	);
	btn_lvl->setSprite(RES::UI_BUTTON);
	btn_lvl->setText("Graj");

	UIButton* btn_scores = new UIButton(
		m_Mgs,
		{ a_ElW, a_ElH + 2 * a_Dy },
		{ a_LogDims.width * .3f, MENU_BTN_H },
		m_ButtonFont
	);
	btn_scores->setSprite(RES::UI_BUTTON);
	btn_scores->setText("Wyniki");

	UIButton* btn_quit = new UIButton(
		m_Mgs,
		{ a_ElW, a_ElH + 3 * a_Dy },
		{ a_LogDims.width * .3f, MENU_BTN_H },
		m_ButtonFont,
		onQuit
	);
	btn_quit->setSprite(RES::UI_BUTTON);
	btn_quit->setText("Wyjscie");

	m_Menu->addElement((UIElement*)btn_lvl);
	m_Menu->addElement((UIElement*)btn_scores);
	m_Menu->addElement((UIElement*)btn_quit);
}

void MenuScene::loadLevelButtons(float a_ElW, float a_ElH, float a_Dy, const Dims& a_LogDims)
{
	auto onLvl1 = [](SDL_Event& ev, UIButton* button, Managers* m_Mgs) { onLevelChange(1, ev, button, m_Mgs); };
	auto onLvl2 = [](SDL_Event& ev, UIButton* button, Managers* m_Mgs) { onLevelChange(2, ev, button, m_Mgs); };

	UIButton* btn_lvl1 = new UIButton(
		m_Mgs,
		{ a_ElW, a_ElH + a_Dy },
		{ a_LogDims.width * .3f, MENU_BTN_H },
		m_ButtonFont,
		onLvl1
	);
	btn_lvl1->setSprite(RES::UI_BUTTON);
	btn_lvl1->setText("Etap 1");

	UIButton* btn_lvl2 = new UIButton(
		m_Mgs,
		{ a_ElW, a_ElH + 2 * a_Dy },
		{ a_LogDims.width * .3f, MENU_BTN_H },
		m_ButtonFont,
		onLvl2
	);
	btn_lvl2->setSprite(RES::UI_BUTTON);
	btn_lvl2->setText("Etap 2");

	m_Menu->addElement((UIElement*)btn_lvl1);
	m_Menu->addElement((UIElement*)btn_lvl2);
}