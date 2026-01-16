#include "level_scene.h"

#include <cstdio>
#include <cassert>
#include "../gameObjects/player.h"
#include "../gameObjects/doyle.h"
#include "../gameObjects/autumn.h"

#define MENU_ASSETS "game/assets/levels/menu/"
#define UI_ASSETS "game/assets/ui/"

static void onNextLevel(SDL_Event& ev, UIButton* button, Managers* m_Mgs) {
	
};

static void onRestart(SDL_Event& ev, UIButton* button, Managers* m_Mgs) {
	
};

static void onQuit(SDL_Event& ev, UIButton* button, Managers* m_Mgs) {
	if (m_Mgs->scene->getCurrentSceneIdx() == SceneID::LEVEL) {
		LevelScene* scene = (LevelScene*)m_Mgs->scene->getCurrentScene();
		scene->gameOverModal();
	}
};

static void onExitToMenu(SDL_Event& ev, UIButton* button, Managers* m_Mgs) {
	m_Mgs->scene->load(SceneID::MENU, false);
};

static constexpr float COMBO_FADE = .3f;
void LevelScene::setFontsAlpha(float a_DiplayTimer)
{
	float progress = a_DiplayTimer / COMBO_DISPLAY_TIME;
	if (progress < COMBO_FADE) {
		m_ComboMulFont.color.a = (Uint8)((progress / COMBO_FADE) * 0XFF);
		m_ComboMulFont.outline.color.a = (Uint8)((progress / COMBO_FADE) * 0XFF);
		m_ComboTxtFont.color.a = (Uint8)((progress / COMBO_FADE) * 0XFF);
		m_ComboTxtFont.outline.color.a = (Uint8)((progress / COMBO_FADE) * 0XFF);
		m_ComboHitsFont.color.a = (Uint8)((progress / COMBO_FADE) * 0XFF);
		m_ComboHitsFont.outline.color.a = (Uint8)((progress / COMBO_FADE) * 0XFF);
	}
	else {
		m_ComboMulFont.color.a = 0XFF;
		m_ComboMulFont.outline.color.a = 0XFF;
		m_ComboTxtFont.color.a = 0XFF;
		m_ComboTxtFont.outline.color.a = 0XFF;
		m_ComboHitsFont.color.a = 0XFF;
		m_ComboHitsFont.outline.color.a = 0XFF;
	}
}

LevelScene::LevelScene(Managers* a_Managers, GameState* a_GameState)
	: GameScene(a_Managers), m_GameState(a_GameState) 
{
}

void LevelScene::start()
{
	m_Mgs->display->showCursor(false);

	m_Mgs->sprite->load(MENU_ASSETS "boxes.bmp", RES::MENU_BOXES);
	m_Mgs->sprite->load(MENU_ASSETS "buildings.bmp", RES::MENU_BUILDINGS);
	m_Mgs->sprite->load(MENU_ASSETS "road.bmp", RES::MENU_ROAD);
	m_Mgs->sprite->load(MENU_ASSETS "sky.bmp", RES::MENU_SKY);
	m_Mgs->sprite->load(MENU_ASSETS "wall1.bmp", RES::MENU_WALL1);
	m_Mgs->sprite->load(MENU_ASSETS "wall2.bmp", RES::MENU_WALL2);
	m_Mgs->sprite->load(MENU_ASSETS "wheels.bmp", RES::MENU_WHEELS);

	m_Mgs->sprite->load(UI_ASSETS "hb_frame.bmp", RES::HB_FRAME);
	m_Mgs->sprite->load(UI_ASSETS "hb_fill.bmp", RES::HB_FILL);
	m_Mgs->sprite->load(UI_ASSETS "hb_ghost.bmp", RES::HB_GHOST);

	addLayer(RES::MENU_SKY, 0.1f, 1280.0f, 720.0f);
	addLayer(RES::MENU_BUILDINGS, 0.2f, 1280.0f, 720.0f);
	addLayer(RES::MENU_WALL2, 1.0f, 1280.0f, 720.0f);
	addLayer(RES::MENU_WALL1, 1.0f, 1280.0f, 720.0f);
	addLayer(RES::MENU_BOXES, 1.0f, 1280.0f, 720.0f);
	addLayer(RES::MENU_WHEELS, 1.0f, 1280.0f, 720.0f);
	addLayer(RES::MENU_ROAD, 1.0f, 2000.0f, 720.f);
	
	Transform tr = {
		{0,0,350},
		0.0,
		NO_FLIP,
		{2.5f, 2.5f}
	};

	m_Player = new Player(m_Mgs, m_GameState, tr);

	m_Camera = new PlayerCamera(m_Mgs, m_Player, { 0,0,500.0f });
	m_Mgs->display->setActiveCamera((Camera*)m_Camera);

	Transform doyleTr = {
		{300,0,350},
		0.0,
		H_FLIP,
		{2.5f, 2.5f}
	};

	Transform autumnTr = {
		{-300,0,350},
		0.0,
		NO_FLIP,
		{2.5f, 2.7f}
	};

	for (int i = 0; i < 10; i++) {
		auto doyle = new Doyle(m_Mgs, m_GameState, (Actor*)m_Player, doyleTr);
		auto autumn = new Autumn(m_Mgs, m_GameState, (Actor*)m_Player, autumnTr);
	}

	m_GameState->reset();

	m_Healthbar = new UIHealthbar(m_Mgs, { 5, 50 }, { 300, 45}, {19, 0});
	m_Healthbar->setMax(m_Player->getHP());
	m_Healthbar->linkVals(&m_Player->getHP());
	m_Healthbar->setSprite(RES::HB_FRAME);
	m_Healthbar->setFill(RES::HB_FILL);
	m_Healthbar->setGhost(RES::HB_GHOST);
	m_Mgs->ui->add((UIElement*)m_Healthbar);
}

void LevelScene::update(float a_Dt)
{
	if (m_Mgs->input->getKeyDown(SDL_SCANCODE_P)) {
		if (m_Paused)
			destroyModal();
		else
			pauseModal();
	}

	if (m_Paused) return;
	m_Mgs->object->updateAll(a_Dt);
}

void LevelScene::fixedUpdate(float a_FixedDt)
{
	if (m_Paused) return;
	m_Mgs->object->fixedUpdateAll(a_FixedDt);
	m_GameState->tick(a_FixedDt);
}

void LevelScene::draw()
{
	drawBackground();
	m_Mgs->object->drawAll();

	if (m_Mgs->engine->inDebugMode()) {
		m_Player->getIBuffer().drawBuffer();
		m_Player->drawActionName();

		for (auto& obj : m_Mgs->object->getAllObjects()) {
			if (!(obj->getType() & ObjectType::ACTOR)) continue;

			Actor* actor = (Actor*)obj;
			actor->drawCollBoxes();
		}
	}

	float comboTimer = m_GameState->getDisplayTimer();
	if (comboTimer > 0.0f)
		drawComboMul(comboTimer);
}

static constexpr float COMBO_YPOS = .05f;
static constexpr float COMBO_HITS_DY = .7f;
static constexpr float COMBO_TXT_DY = .9f;

void LevelScene::drawComboMul(float a_DiplayTimer)
{
	auto currStatus = m_GameState->getCurrentStatus();
	if (currStatus == nullptr) return;

	setFontsAlpha(a_DiplayTimer);

	float txtW;
	float dy = m_ComboMulFont.chSize * m_ComboMulFont.scale;
	Dims logDims = m_Mgs->display->getLogDims();

	char hits[UINT8_MAXLEN]{};
	snprintf(hits, UINT8_MAXLEN, "%d", currStatus->threshold);
	txtW = m_ComboMulFont.getWidth((int)strlen(hits));

	m_Mgs->display->drawString(
		{
			(logDims.width / 2.0f) - (txtW / 2.0f) + (COMBO_XOFF * 3.0f),
			logDims.height * COMBO_YPOS
		},
		hits,
		m_ComboMulFont
	);

	txtW = m_ComboHitsFont.getWidth((int)strlen(HITS_LIT));

	m_Mgs->display->drawString(
		{
			(logDims.width / 2.0f) + (txtW / 2.0f) - COMBO_XOFF,
			logDims.height * COMBO_YPOS + dy * COMBO_HITS_DY
		},
		HITS_LIT,
		m_ComboHitsFont
	);

	txtW = m_ComboTxtFont.getWidth((int)strlen(currStatus->text));

	m_Mgs->display->drawString(
		{
			(logDims.width / 2.0f) - (txtW/2.0f) + COMBO_XOFF,
			logDims.height * COMBO_YPOS + dy * COMBO_TXT_DY
		},
		currStatus->text, 
		m_ComboTxtFont
	);
}

void LevelScene::destroy()
{
	m_Mgs->object->clear();

	if (m_Mgs->ui) m_Mgs->ui->clear();
	m_Healthbar = nullptr;
	m_Player = nullptr;
	m_Camera = nullptr;
	m_Modal = nullptr;
	m_Paused = false;

	m_Mgs->sprite->unload(RES::MENU_BOXES);
	m_Mgs->sprite->unload(RES::MENU_BUILDINGS);
	m_Mgs->sprite->unload(RES::MENU_ROAD);
	m_Mgs->sprite->unload(RES::MENU_SKY);
	m_Mgs->sprite->unload(RES::MENU_WALL1);
	m_Mgs->sprite->unload(RES::MENU_WALL2);
	m_Mgs->sprite->unload(RES::MENU_WHEELS);

	m_Mgs->sprite->unload(RES::HB_FRAME);
	m_Mgs->sprite->unload(RES::HB_FILL);
	m_Mgs->sprite->unload(RES::HB_GHOST);
}

void LevelScene::createModal()
{
	m_Paused = true;
	m_Mgs->display->showCursor(true);

	const Dims& logDims = m_Mgs->display->getLogDims();
	m_Modal = new UISpriteContainer(
		m_Mgs,
		{ logDims.width * .23f, logDims.height * .18f },
		{ logDims.width * .54f, logDims.height * .64f }
	);
	m_Modal->setSprite(RES::UI_BIG_FRAME);

	m_Mgs->ui->add(m_Modal);
}

void LevelScene::destroyModal()
{
	m_Mgs->ui->remove(m_Modal);
	m_Modal = nullptr;

	m_Mgs->display->showCursor(false);
	m_Paused = false;
}

void LevelScene::pauseModal()
{
	if (m_Modal != nullptr)
		destroyModal();
	createModal();

	const Dims& logDims = m_Mgs->display->getLogDims();
	float elW = logDims.width * BTN_X_MUL;
	float elH = logDims.height * BTN_Y_MUL;
	float dy = MENU_BTN_H + MENU_BTN_GAP;
	const float ch_size = TITLE_FONT.chSize * TITLE_FONT.scale;
	const char* t_text = "Pauza";
	const float t_size = TITLE_FONT.getWidth((int)strlen(t_text));
	const float t_x = (logDims.width / 2.0f) - (t_size / 2.0f);

	UITextElement* pauseTxt = new UITextElement(
		m_Mgs,
		{ t_x, elH - dy },
		{ t_size, ch_size },
		TITLE_FONT
	);
	pauseTxt->setText(t_text);

	m_Modal->addElement((UIElement*)pauseTxt);
}

static constexpr int SCORE_MAX_LEN = 64;
void LevelScene::endModal(bool hasWon)
{
	if (m_Modal != nullptr)
		destroyModal();
	createModal();

	const char* t_text;
	if (hasWon)
		t_text = "Etap ukonczony!";
	else
		t_text = "Porazka!";

	const Dims& logDims = m_Mgs->display->getLogDims();

	float elW = logDims.width * BTN_X_MUL;
	float elH = logDims.height * BTN_Y_MUL;
	float dy = MENU_BTN_H + MENU_BTN_GAP;
	float ch_size = TITLE_FONT.chSize * TITLE_FONT.scale;
	float t_size = TITLE_FONT.getWidth((int)strlen(t_text));
	float t_x = (logDims.width / 2.0f) - (t_size / 2.0f);

	UITextElement* endTxt = new UITextElement(
		m_Mgs,
		{ t_x, elH - dy },
		{ t_size, ch_size },
		TITLE_FONT
	);
	endTxt->setText(t_text);

	char scoreBuff[SCORE_MAX_LEN + 1]{};
	snprintf(scoreBuff, SCORE_MAX_LEN, "Wynik: %01d$", m_GameState->getScore());
	t_size = H2_FONT.getWidth((int)strlen(scoreBuff));
	t_x = (logDims.width / 2.0f) - (t_size / 2.0f);

	UITextElement* scoreTxt = new UITextElement(
		m_Mgs,
		{ t_x, elH },
		{ t_size, ch_size },
		H2_FONT
	);
	scoreTxt->setText(scoreBuff);

	UIButton* continueBtn = new UIButton(
		m_Mgs,
		{ elW, elH + dy },
		{ logDims.width * .3f, MENU_BTN_H },
		BUTTON_FONT,
		(hasWon) ? onNextLevel : onRestart
	);
	continueBtn->setSprite(RES::UI_BUTTON);
	continueBtn->setText("Kontynuuj");

	UIButton* quitBtn = new UIButton(
		m_Mgs,
		{ elW, elH + 2 * dy },
		{ logDims.width * .3f, MENU_BTN_H },
		BUTTON_FONT,
		onQuit
	);
	quitBtn->setSprite(RES::UI_BUTTON);
	quitBtn->setText("Wyjdz");

	m_Modal->addElement((UIElement*)endTxt);
	m_Modal->addElement((UIElement*)scoreTxt);
	m_Modal->addElement((UIElement*)continueBtn);
	m_Modal->addElement((UIElement*)quitBtn);
}

void LevelScene::gameOverModal()
{
	if (m_Modal != nullptr)
		destroyModal();
	createModal();

	const Dims& logDims = m_Mgs->display->getLogDims();

	float elW = logDims.width * BTN_X_MUL;
	float elH = logDims.height * BTN_Y_MUL;
	float dy = MENU_BTN_H + MENU_BTN_GAP;
	const float ch_size = TITLE_FONT.chSize * TITLE_FONT.scale;
	const char* t_text = "Koniec gry!";
	float t_size = TITLE_FONT.getWidth((int)strlen(t_text));
	float t_x = (logDims.width / 2.0f) - (t_size / 2.0f);

	UITextElement* endTxt = new UITextElement(
		m_Mgs,
		{ t_x, elH - dy },
		{ t_size, ch_size },
		TITLE_FONT
	);
	endTxt->setText(t_text);

	char scoreBuff[SCORE_MAX_LEN + 1]{};

	snprintf(scoreBuff, SCORE_MAX_LEN, "Wynik: %01d$", m_GameState->getScore());

	t_size = H2_FONT.getWidth((int)strlen(scoreBuff));
	t_x = (logDims.width / 2.0f) - (t_size / 2.0f);

	UITextElement* scoreTxt = new UITextElement(
		m_Mgs,
		{ t_x, elH },
		{ t_size, ch_size },
		H2_FONT
	);
	scoreTxt->setText(scoreBuff);

	UIButton* continueBtn = new UIButton(
		m_Mgs,
		{ elW, elH + dy },
		{ logDims.width * .3f, MENU_BTN_H },
		BUTTON_FONT
	);
	continueBtn->setSprite(RES::UI_BUTTON);
	continueBtn->setText("Zapisz");

	UIButton* quitBtn = new UIButton(
		m_Mgs,
		{ elW, elH + 2 * dy },
		{ logDims.width * .3f, MENU_BTN_H },
		BUTTON_FONT,
		onExitToMenu
	);
	quitBtn->setSprite(RES::UI_BUTTON);
	quitBtn->setText("Wroc do menu");

	m_Modal->addElement((UIElement*)endTxt);
	m_Modal->addElement((UIElement*)scoreTxt);
	m_Modal->addElement((UIElement*)continueBtn);
	m_Modal->addElement((UIElement*)quitBtn);
}

void LevelScene::loadFromFile(const char* a_FileName)
{
	assert(m_Mgs->scene->getCurrentScene() == this && "Scene should be loaded before being displayed!");
}