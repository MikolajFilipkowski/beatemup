#include "menu_scene.h"

#include <cstring>
#include <cstdio>
#include "level_scene.h"

#define BG_ASSETS "./game/assets/backgrounds/default/"
#define UI_ASSETS "./game/assets/ui/"

static void onLevelSelector(SDL_Event& ev, UIButton* button, Managers* m_Mgs) {
	if (m_Mgs->scene->getCurrentSceneIdx() == SceneID::MENU) {
		MenuScene* scene = (MenuScene*)m_Mgs->scene->getCurrentScene();
		scene->setPlayerName();
		scene->setNextState(MenuState::LEVEL_SEL);
	}
};

static void onScoreViewer(SDL_Event& ev, UIButton* button, Managers* m_Mgs) {
	if (m_Mgs->scene->getCurrentSceneIdx() == SceneID::MENU) {
		MenuScene* scene = (MenuScene*)m_Mgs->scene->getCurrentScene();
		scene->setNextState(MenuState::SCORES);
	}
};

static void onBack(SDL_Event& ev, UIButton* button, Managers* m_Mgs) {
	if (m_Mgs->scene->getCurrentSceneIdx() == SceneID::MENU) {
		MenuScene* scene = (MenuScene*)m_Mgs->scene->getCurrentScene();
		scene->setNextState(MenuState::MAIN);
	}
};

static void onQuit(SDL_Event& ev, UIButton* button, Managers* m_Mgs) {
	m_Mgs->engine->stop();
};

static void onLevelChange(SDL_Event& ev, UIButton* button, Managers* m_Mgs) {
	auto lvlSc = (LevelScene*)m_Mgs->scene->get(SceneID::LEVEL);
	int id = button->getId();
	bool success = lvlSc->loadFromFile(id);
	if (!success) {
		m_Mgs->engine->throwError("Failed to load scene %d", id);
		return;
	}
	m_Mgs->scene->load(SceneID::LEVEL, false);
}

void MenuScene::getElInfo(const Dims& a_LogDims, float& a_ElW, float& a_ElH, float& a_Dy)
{
	a_ElW = a_LogDims.width * BTN_X_MUL;
	a_ElH = a_LogDims.height * BTN_Y_MUL;
	a_Dy = MENU_BTN_H + MENU_BTN_GAP;
}

MenuScene::MenuScene(Managers* a_Managers, GameState* a_GameState, GameLoader* a_GameLoader, GameSettings& a_Settings)
	: GameScene(a_Managers), m_GameState(a_GameState), m_GameLoader(a_GameLoader), m_Settings(a_Settings) {}

void MenuScene::start()
{
	const Dims log_dims = m_Mgs->display->getLogDims();
	m_Mgs->display->showCursor(true);

	loadBackgrounds();
	loadMainMenu(log_dims);

	m_Cam = new Camera(m_Mgs);
	m_Mgs->display->setActiveCamera(m_Cam);
}

static constexpr float MENU_CAM_SCROLL_SPD = 60.0f;
void MenuScene::update(float a_Dt)
{
	if (m_TextInput == nullptr && m_Mgs->input->getKeyDown(SDL_SCANCODE_Q)) {
		m_NextState = MenuState::MAIN;
	}
	if (m_NextState != MenuState::NONE) {
		changeState(m_NextState);
		return;
	}

	if (m_Cam == nullptr) return;
	Vector3 pos = m_Cam->getPosition();
	m_Cam->setPosition({pos.x + MENU_CAM_SCROLL_SPD * a_Dt, pos.y, pos.z});
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

	m_Mgs->sprite->unload(RES::BG_ELEMENT1);
	m_Mgs->sprite->unload(RES::BG_BUILDINGS);
	m_Mgs->sprite->unload(RES::BG_ROAD);
	m_Mgs->sprite->unload(RES::BG_SKY);
	m_Mgs->sprite->unload(RES::BG_WALL1);
	m_Mgs->sprite->unload(RES::BG_WALL2);
	m_Mgs->sprite->unload(RES::BG_ELEMENT2);
}

void MenuScene::changeState(MenuState a_State)
{
	const Dims log_dims = m_Mgs->display->getLogDims();

	m_Mgs->ui->remove(m_Menu);
	m_Menu = nullptr;
	SDL_StopTextInput();

	switch (m_NextState) {
	case MenuState::MAIN:
		loadMainMenu(log_dims);
		break;
	case MenuState::LEVEL_SEL:
		loadLevelSelector(log_dims);
		break;
	case MenuState::SCORES:
		loadScoreViewer(log_dims);
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

void MenuScene::setPlayerName()
{
	if (m_TextInput == nullptr) return;
	m_GameState->setPlayerName(m_TextInput->getContent());
}

static constexpr float MENU_RECT_X = .23f;
static constexpr float MENU_RECT_W = 1.0f - 2 * MENU_RECT_X;
static constexpr float MENU_RECT_Y = .18f;
static constexpr float MENU_RECT_H = 1.0f - 2 * MENU_RECT_Y;
void MenuScene::createMenuCont(const Dims& a_LogDims)
{
	m_Menu = new UISpriteContainer(
		m_Mgs,
		{ a_LogDims.width * MENU_RECT_X, a_LogDims.height * MENU_RECT_Y },
		{ a_LogDims.width * MENU_RECT_W, a_LogDims.height * MENU_RECT_H }
	);
	m_Menu->setSprite(RES::UI_BIG_FRAME);
}

static constexpr float CONT_RECT_X = .25f;
static constexpr float CONT_RECT_W = 1.0f - 2 * CONT_RECT_X;
static constexpr float CONT_RECT_Y = .35f;
static constexpr float CONT_RECT_H = 1.0f - 2 * CONT_RECT_Y;
static constexpr Vector2 TEXT_EL_OFFSETT = Vector2(5, 14);

void MenuScene::createScrollCont(const Dims& a_LogDims, Highscore* a_Scores, int a_Count)
{
	Rect contRect = {
		a_LogDims.width * CONT_RECT_X, a_LogDims.height * CONT_RECT_Y,
		a_LogDims.width * CONT_RECT_W, a_LogDims.height * CONT_RECT_H
	};
	auto scrollCont = new UIScrollableContainer(
		m_Mgs,
		{ contRect.x, contRect.y },
		{ contRect.w, contRect.h },
		{0,2}
	);
	scrollCont->setBackground(SCROLL_AREA_CLR);
	m_Menu->addElement((UIElement*)scrollCont);

	if (a_Scores == nullptr || a_Count == 0) return;

	char buff[MAX_TEXTSIZE];
	for (int i = 0; i < a_Count; i++) {
		FDims size = { contRect.w, contRect.h * .2f };
		Vector2 pos = { 
			(contRect.w - size.width)/2.0f, 
			(size.height) * i
		};

		auto sprCont = new UISpriteContainer(m_Mgs, pos, size);
		sprCont->setBorder(Colors::black, 1);

		auto scoreTxt = new UITextElement(m_Mgs,
			{
				pos.x,
				contRect.y
			}, 
			size,
			BUTTON_FONT
		);
		scoreTxt->setFollow(true);
		scoreTxt->leftPos(pos + TEXT_EL_OFFSETT, size);
		snprintf(buff, MAX_TEXTSIZE, "%s: %d", a_Scores[i].name, a_Scores[i].score);

		scoreTxt->setText(buff);
		sprCont->addElement(scoreTxt);

		scrollCont->addElement((UIElement*)sprCont);
	}
}


void MenuScene::loadBackgrounds()
{
	m_Mgs->sprite->load(BG_ASSETS "element1.bmp", RES::BG_ELEMENT1);
	m_Mgs->sprite->load(BG_ASSETS "buildings.bmp", RES::BG_BUILDINGS);
	m_Mgs->sprite->load(BG_ASSETS "road.bmp", RES::BG_ROAD);
	m_Mgs->sprite->load(BG_ASSETS "sky.bmp", RES::BG_SKY);
	m_Mgs->sprite->load(BG_ASSETS "wall1.bmp", RES::BG_WALL1);
	m_Mgs->sprite->load(BG_ASSETS "wall2.bmp", RES::BG_WALL2);
	m_Mgs->sprite->load(BG_ASSETS "element2.bmp", RES::BG_ELEMENT2);

	addLayer(RES::BG_SKY, BG_SKY_SPD, BG_DEF_W, BG_DEF_H);
	addLayer(RES::BG_BUILDINGS, BG_BLD_SPD, BG_DEF_W, BG_DEF_H);
	addLayer(RES::BG_WALL2, BG_DEF_SPD, BG_DEF_W, BG_DEF_H);
	addLayer(RES::BG_WALL1, BG_DEF_SPD, BG_DEF_W, BG_DEF_H);
	addLayer(RES::BG_ELEMENT1, BG_DEF_SPD, BG_DEF_W, BG_DEF_H);
	addLayer(RES::BG_ELEMENT2, BG_DEF_SPD, BG_DEF_W, BG_DEF_H);
	addLayer(RES::BG_ROAD, BG_DEF_SPD, BG_ROAD_W, BG_DEF_H);
}


static constexpr float TITLE_Y = .225f;
void MenuScene::loadTitle(const Dims& a_LogDims)
{
	m_Mgs->sprite->setSpriteColor(RES::PUNCH, TITLE_FONT.color);

	const float ch_size = TITLE_FONT.chSize * TITLE_FONT.scale;
	const char* t_text = "Beat em up";
	const float t_size = (strlen(t_text)) * ch_size * TITLE_FONT.spacing;

	const float t_x = (a_LogDims.width / 2.0f) - (t_size / 2.0f);
	const float t_y = a_LogDims.height * TITLE_Y;

	UISpriteContainer* punch = new UISpriteContainer(
		m_Mgs, { t_x + IC_OFF_X, t_y + IC_OFF_Y }, { IC_SIZE, IC_SIZE }
	);
	punch->setSprite(RES::PUNCH);

	UITextElement* title = new UITextElement(
		m_Mgs,
		{ t_x, t_y },
		{ t_size, ch_size },
		TITLE_FONT
	);

	title->setText(t_text);
	m_Menu->addElement((UIElement*)punch);
	m_Menu->addElement((UIElement*)title);
}

static constexpr float TXTIN_X = .35f;
static constexpr float TXTIN_Y = .32f;
static constexpr float TXTIN_W = .3f;
static constexpr float TXTIN_H = 1.5f;
static constexpr Vector2 TXTIN_PADD = { 50, 0 };
void MenuScene::loadMainMenu(const Dims& a_LogDims)
{
	createMenuCont(a_LogDims);
	loadTitle(a_LogDims);

	float el_w, el_h, dy;
	getElInfo(a_LogDims, el_w, el_h, dy);

	m_TextInput = new UITextInput(
		m_Mgs,
		{ a_LogDims.width * TXTIN_X, a_LogDims.height * TXTIN_Y },
		{ a_LogDims.width * TXTIN_W, MENU_BTN_H * TXTIN_H },
		INPUT_FONT,
		MAX_PLY_LEN,
		TXTIN_PADD
	);

	m_TextInput->setSprite(RES::UI_TEXT_INPUT);
	m_TextInput->setPlaceholder(PLACEHOLDER_TEXT, PLACEHOLDER_CLR);

	m_Menu->addElement((UIElement*)m_TextInput);

	loadMainButtons(el_w, el_h, dy, a_LogDims);

	m_Mgs->ui->add((UIElement*)m_Menu);
}

void MenuScene::loadLevelSelector(const Dims& a_LogDims)
{
	m_Mgs->ui->remove(m_Menu);
	m_TextInput = nullptr;
	
	createMenuCont(a_LogDims);

	m_Mgs->ui->add(m_Menu);

	loadTitle(a_LogDims);

	float el_w, el_h, dy;
	getElInfo(a_LogDims, el_w, el_h, dy);

	loadLevelButtons(el_w, el_h, dy, a_LogDims);
}

static constexpr float BTN_W = .3f;
static constexpr float BACK_BTN_POS = 3.5f;
void MenuScene::loadScoreViewer(const Dims& a_LogDims)
{
	int count;
	auto scores = m_GameLoader->loadScores(count);

	m_Mgs->ui->remove(m_Menu);
	m_TextInput = nullptr;

	createMenuCont(a_LogDims);
	createScrollCont(a_LogDims, scores, count);

	float el_w, el_h, dy;
	getElInfo(a_LogDims, el_w, el_h, dy);

	UIButton* backBtn = new UIButton(
		m_Mgs,
		{ el_w, el_h + BACK_BTN_POS * dy},
		{ a_LogDims.width * BTN_W, MENU_BTN_H },
		BUTTON_FONT,
		onBack
	);
	backBtn->setSprite(RES::UI_BUTTON);
	backBtn->setText("Cofnij");

	m_Menu->addElement((UIElement*)backBtn);
	m_Mgs->ui->add(m_Menu);

	loadTitle(a_LogDims);
}

void MenuScene::loadMainButtons(float a_ElW, float a_ElH, float a_Dy, const Dims& a_LogDims)
{
	UIButton* lvlBtn = new UIButton(
		m_Mgs,
		{ a_ElW, a_ElH + a_Dy },
		{ a_LogDims.width * BTN_W, MENU_BTN_H },
		BUTTON_FONT,
		onLevelSelector
	);
	lvlBtn->setSprite(RES::UI_BUTTON);
	lvlBtn->setText("Graj");

	UIButton* scoresBtn = new UIButton(
		m_Mgs,
		{ a_ElW, a_ElH + 2 * a_Dy },
		{ a_LogDims.width * BTN_W, MENU_BTN_H },
		BUTTON_FONT,
		onScoreViewer
	);
	scoresBtn->setSprite(RES::UI_BUTTON);
	scoresBtn->setText("Wyniki");

	UIButton* quitBtn = new UIButton(
		m_Mgs,
		{ a_ElW, a_ElH + 3 * a_Dy },
		{ a_LogDims.width * BTN_W, MENU_BTN_H },
		BUTTON_FONT,
		onQuit
	);
	quitBtn->setSprite(RES::UI_BUTTON);
	quitBtn->setText("Wyjscie");

	m_Menu->addElement((UIElement*)lvlBtn);
	m_Menu->addElement((UIElement*)scoresBtn);
	m_Menu->addElement((UIElement*)quitBtn);
}

void MenuScene::loadLevelButtons(float a_ElW, float a_ElH, float a_Dy, const Dims& a_LogDims)
{
	char buff[BTN_TEXTSIZE]{};

	UIButton* backBtn = new UIButton(
		m_Mgs,
		{ a_ElW, a_ElH + BACK_BTN_POS * a_Dy },
		{ a_LogDims.width * BTN_W, MENU_BTN_H },
		BUTTON_FONT,
		onBack
	);
	backBtn->setSprite(RES::UI_BUTTON);
	backBtn->setText("Cofnij");
	m_Menu->addElement((UIElement*)backBtn);

	for (Uint32 i = 0; i < m_Settings.levels; i++) {
		UIButton* btn = new UIButton(
			m_Mgs,
			{ a_ElW, a_ElH + a_Dy * i },
			{ a_LogDims.width * BTN_W, MENU_BTN_H },
			BUTTON_FONT,
			onLevelChange
		);

		btn->setId(i + 1);
		btn->setSprite(RES::UI_BUTTON);
		snprintf(buff, BTN_TEXTSIZE, "Etap %d", i + 1);
		btn->setText(buff);

		m_Menu->addElement((UIElement*)btn);
	}
}