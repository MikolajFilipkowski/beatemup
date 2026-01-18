#include "level_scene.h"

#include <cstdio>
#include <cassert>
#include "../gameObjects/player.h"
#include "../gameObjects/doyle.h"
#include "../gameObjects/autumn.h"

#define BG_ASSETS "game/assets/backgrounds/"
#define UI_ASSETS "game/assets/ui/"
#define OBS_ASSETS "game/assets/obstacles/"

static void onNextLevel(SDL_Event& ev, UIButton* button, Managers* m_Mgs) {
	auto lvlSc = (LevelScene*)m_Mgs->scene->get(SceneID::LEVEL);
	lvlSc->changeLevel(button->getId() + 1, true);
};

static void onRestart(SDL_Event& ev, UIButton* button, Managers* m_Mgs) {
	auto lvlSc = (LevelScene*)m_Mgs->scene->get(SceneID::LEVEL);
	lvlSc->changeLevel(button->getId());
};

static void onSave(SDL_Event& ev, UIButton* button, Managers* m_Mgs) {
	if (m_Mgs->scene->getCurrentSceneIdx() == SceneID::LEVEL) {
		LevelScene* scene = (LevelScene*)m_Mgs->scene->getCurrentScene();
		scene->saveScore();
		m_Mgs->scene->load(SceneID::MENU, false);
	}
}

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

void LevelScene::reinitializeScene()
{
	if (m_LevelQueued == 0) return;

	int score = 0;
	if (m_PreserveScore)
		score = m_GameState->getScore();

	destroy();
	if (loadFromFile(m_LevelQueued))
		start();

	m_GameState->setScore(score);

	m_LevelQueued = 0;
}

LoadedZone* LevelScene::getZone(float a_PosX) const
{
	for (Uint32 i = 0; i < m_LoadedLevel->zoneCount; i++) {
		LoadedZone& zone = m_LoadedLevel->zones[i];

		if (zone.beginning.x <= a_PosX && zone.end.x >= a_PosX)
			return &zone;
	}
	return &(m_LoadedLevel->zones[0]);
}

LoadedZone* LevelScene::getWinZone() const
{
	for (Uint32 i = 0; i < m_LoadedLevel->zoneCount; i++) {
		LoadedZone& zone = m_LoadedLevel->zones[i];

		if (zone.flags & WIN_FLAG)
			return &zone;
	}
	return nullptr;
}


void LevelScene::loadGraphics()
{
	// Wczytywanie warstw tla
	loadLayer(RES::BG_ELEMENT1, BG_ASSETS, m_LoadedLevel->background, "element1.bmp");
	loadLayer(RES::BG_BUILDINGS, BG_ASSETS, m_LoadedLevel->background, "buildings.bmp");
	loadLayer(RES::BG_ROAD, BG_ASSETS, m_LoadedLevel->background, "road.bmp");
	loadLayer(RES::BG_SKY, BG_ASSETS, m_LoadedLevel->background, "sky.bmp");
	loadLayer(RES::BG_WALL1, BG_ASSETS, m_LoadedLevel->background, "wall1.bmp");
	loadLayer(RES::BG_WALL2, BG_ASSETS, m_LoadedLevel->background, "wall2.bmp");
	loadLayer(RES::BG_ELEMENT2, BG_ASSETS, m_LoadedLevel->background, "element2.bmp");

	m_Mgs->sprite->load(UI_ASSETS "hb_frame.bmp", RES::HB_FRAME);
	m_Mgs->sprite->load(UI_ASSETS "hb_fill.bmp", RES::HB_FILL);
	m_Mgs->sprite->load(UI_ASSETS "hb_ghost.bmp", RES::HB_GHOST);
	m_Mgs->sprite->load(OBS_ASSETS "barrel.bmp", RES::BARREL);

	addLayer(RES::BG_SKY, BG_SKY_SPD, BG_DEF_W, BG_DEF_H);
	addLayer(RES::BG_BUILDINGS, BG_BLD_SPD, BG_DEF_W, BG_DEF_H);
	addLayer(RES::BG_WALL2, BG_DEF_SPD, BG_DEF_W, BG_DEF_H);
	addLayer(RES::BG_WALL1, BG_DEF_SPD, BG_DEF_W, BG_DEF_H);
	addLayer(RES::BG_ELEMENT1, BG_DEF_SPD, BG_DEF_W, BG_DEF_H);
	addLayer(RES::BG_ELEMENT2, BG_DEF_SPD, BG_DEF_W, BG_DEF_H);
	addLayer(RES::BG_ROAD, BG_DEF_SPD, BG_ROAD_W, BG_DEF_H);
}

LevelScene::LevelScene(Managers* a_Managers, GameState* a_GameState, GameLoader* a_GameLoader, GameSettings& a_Settings)
	: GameScene(a_Managers), m_GameState(a_GameState), m_GameLoader(a_GameLoader), m_Settings(a_Settings)
{
}

static constexpr Vector2 HEALTHBAR_POS = { 5, 50 };
static constexpr FDims HEALTHBAR_DIMS = { 300, 45 };
static constexpr Vector2 HEALTHBAR_PADD = { 19, 0 };
void LevelScene::start()
{
	if (m_LoadedLevel == nullptr) return;
	m_Mgs->display->showCursor(false);

	loadGraphics();
	
	Transform tr = {
		{PLY_POS_X, 0, PLY_POS_Z},
		0.0,
		NO_FLIP,
		{ACTOR_SCALE, ACTOR_SCALE}
	};

	m_Player = new Player(m_Mgs, m_GameState, tr);

	m_Camera = new PlayerCamera(m_Mgs, m_Player, { 0, 0, CAM_POS_Z });
	m_Mgs->display->setActiveCamera((Camera*)m_Camera);
	m_EnemyCount = 0;

	for (Uint32 i = 0; i < m_LoadedLevel->enemyCount; i++) {
		auto enemy = m_LoadedLevel->enemies[i];
		Transform enemyTr = { enemy.pos, 0.0, H_FLIP, {ACTOR_SCALE, ACTOR_SCALE} };
		if (enemy.type == EnemyType::DOYLE) {
			new Doyle(m_Mgs, m_GameState, (Actor*)m_Player, enemyTr, m_EnemyCount);
		}
		else if (enemy.type == EnemyType::AUTUMN) {
			enemyTr.scale.y = AUTUMN_SCALE_Y;
			new Autumn(m_Mgs, m_GameState, (Actor*)m_Player, enemyTr, m_EnemyCount);
		}
	}
	for (Uint32 i = 0; i < m_LoadedLevel->obstacleCount; i++) {
		auto obstacle = m_LoadedLevel->obstacles[i];
		int sprKey = getObstacleSpriteKey(obstacle.type);
		Sprite* spr = m_Mgs->sprite->get(sprKey);
		Vector2 scale = {};
		scale.x = obstacle.dims.x / (float)spr->width;
		scale.y = obstacle.dims.y / (float)spr->height;
		Transform obsTr = {
			obstacle.pos,
			0.0, NO_FLIP, scale
		};
		new Obstacle(m_Mgs, obsTr, obstacle.dims, sprKey, obstacle.mass);
	}

	m_GameState->reset();

	m_Healthbar = new UIHealthbar(m_Mgs, HEALTHBAR_POS, HEALTHBAR_DIMS, HEALTHBAR_PADD);
	m_Healthbar->setMax(m_Player->getHP());
	m_Healthbar->linkVals(&m_Player->getHP());
	m_Healthbar->setSprite(RES::HB_FRAME);
	m_Healthbar->setFill(RES::HB_FILL);
	m_Healthbar->setGhost(RES::HB_GHOST);
	m_Mgs->ui->add((UIElement*)m_Healthbar);
}


void LevelScene::update(float a_Dt)
{
	if (m_LevelQueued != 0) {
		reinitializeScene();
		return;
	}
	if (m_Mgs->input->getKeyDown(SDL_SCANCODE_P)) {
		if (m_Paused)
			destroyModal();
		else
			pauseModal();
	}
	if (m_Mgs->input->getKeyDown(SDL_SCANCODE_F5)) {
		for (auto& obj : m_Mgs->object->getAllObjects()) {
			if ((obj->getType() & ObjectType::ENEMY) == ObjectType::ENEMY) {
				obj->setRemovalFlag(true);
			}
		}
	}

	if (m_Paused) return;
	m_Mgs->object->updateAll(a_Dt);
}

void LevelScene::fixedUpdate(float a_FixedDt)
{
	if (m_LevelQueued != 0) {
		reinitializeScene();
		return;
	}
	if (m_Paused) return;

	for (auto& obj : m_Mgs->object->getAllObjects()) {
		Rigidbody& rb = obj->getRb();
		Bounds& bounds = rb.bounds;
		if (bounds.applyBounds) {
			LoadedZone* zone = getZone(rb.currPos.x);
			rb.bounds.maxX = m_LoadedLevel->width;
			rb.bounds.minX = 0;
			rb.bounds.maxZ = zone->end.y;
			rb.bounds.minZ = zone->beginning.y;
		}
		if (obj->getType() & ObjectType::CAMERA) {
			float vpW = m_Mgs->display->getActiveCamera()->getViewport().w / 2.0f;
			rb.bounds.minX += vpW;
			rb.bounds.maxX -= vpW;
			rb.bounds.minZ = rb.currPos.z;
			rb.bounds.maxZ = rb.currPos.z;
		}
	}

	Vector3 plyPos = m_Player->getRb().currPos;
	LoadedZone* winZone = getWinZone();
	if (winZone && plyPos.x > winZone->beginning.x && 
		plyPos.x < winZone->end.x && m_EnemyCount == 0
	) {
		endModal(true);
	}

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
		m_Player->drawPos();

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
static constexpr float COMBO_OFF_DY = 3.0f;

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
			(logDims.width / 2.0f) - (txtW / 2.0f) + (COMBO_XOFF * COMBO_OFF_DY),
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
	m_EnemyCount = 0;
	m_PreserveScore = false;

	if (m_Mgs->ui) m_Mgs->ui->clear();
	delete m_LoadedLevel;
	m_LoadedLevel = nullptr;
	m_Healthbar = nullptr;
	m_Player = nullptr;
	m_Camera = nullptr;
	m_Modal = nullptr;
	m_Paused = false;

	m_Mgs->sprite->unload(RES::BG_ELEMENT1);
	m_Mgs->sprite->unload(RES::BG_BUILDINGS);
	m_Mgs->sprite->unload(RES::BG_ROAD);
	m_Mgs->sprite->unload(RES::BG_SKY);
	m_Mgs->sprite->unload(RES::BG_WALL1);
	m_Mgs->sprite->unload(RES::BG_WALL2);
	m_Mgs->sprite->unload(RES::BG_ELEMENT2);

	m_Mgs->sprite->unload(RES::HB_FRAME);
	m_Mgs->sprite->unload(RES::HB_FILL);
	m_Mgs->sprite->unload(RES::HB_GHOST);
}

void LevelScene::saveScore() const
{
	m_GameLoader->saveScore(m_GameState);
}

void LevelScene::changeLevel(int a_Id, bool a_PreserveScore)
{
	m_LevelQueued = a_Id;
	m_PreserveScore = a_PreserveScore;
}

static constexpr float MODAL_X = .23f;
static constexpr float MODAL_W = .54f;
static constexpr float MODAL_Y = .18f;
static constexpr float MODAL_H = .64f;
void LevelScene::createModal()
{
	m_Paused = true;
	m_Mgs->display->showCursor(true);

	const Dims& logDims = m_Mgs->display->getLogDims();
	m_Modal = new UISpriteContainer(
		m_Mgs,
		{ logDims.width * MODAL_X, logDims.height * MODAL_Y },
		{ logDims.width * MODAL_W, logDims.height * MODAL_H }
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

	if (!hasWon || m_LoadedLevel->id < (int)m_Settings.levels) {
		UIButton* continueBtn = new UIButton(
			m_Mgs,
			{ elW, elH + dy },
			{ logDims.width * .3f, MENU_BTN_H },
			BUTTON_FONT,
			(hasWon) ? onNextLevel : onRestart
		);
		continueBtn->setId(m_LoadedLevel->id);
		continueBtn->setSprite(RES::UI_BUTTON);
		continueBtn->setText("Kontynuuj");
		m_Modal->addElement((UIElement*)continueBtn);
	}

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

	UIButton* saveBtn = new UIButton(
		m_Mgs,
		{ elW, elH + dy },
		{ logDims.width * .3f, MENU_BTN_H },
		BUTTON_FONT,
		onSave
	);
	saveBtn->setSprite(RES::UI_BUTTON);
	saveBtn->setText("Zapisz");

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
	m_Modal->addElement((UIElement*)saveBtn);
	m_Modal->addElement((UIElement*)quitBtn);
}

bool LevelScene::loadFromFile(int a_Id)
{
	delete m_LoadedLevel;

	m_LoadedLevel = new LoadedLevel();
	return m_GameLoader->loadLevel(a_Id, *m_LoadedLevel);
}