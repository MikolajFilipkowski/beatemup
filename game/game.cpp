#define _CRT_SECURE_NO_WARNINGS

#include "game.h"
#include <cstdio>
#include <ctime>
#include <cstdlib>

#define CH_ASSETS "game/assets/charsets/"
#define UI_ASSETS "game/assets/ui/"

Game::Game(GameLoader& a_GameLoader, GameSettings& a_Settings) 
	: Application(), m_GameLoader(a_GameLoader), m_Settings(a_Settings) 
{
	
}

bool Game::onStart(Managers* a_Managers) {
	m_Mgs = a_Managers;

	srand((unsigned)time(0));

	m_Mgs->display->setBorderless(m_Settings.borderless);
	m_Mgs->display->setResizable(m_Settings.resizable);
	m_Mgs->display->setFullscreen(m_Settings.fullscreen);
	m_Mgs->display->setIcon("game/assets/punch_icon.bmp");

	setupBindings();
	setupStatuses();
	loadCharsets();
	loadUITextures();

	m_Mgs->sprite->load("game/assets/circle.bmp", RES::CIRCLE);
	m_Mgs->sprite->load("game/assets/shadow.bmp", RES::SHADOW);

	m_GameLoader.loadActionData("game/data/actions.cfg");

	m_Mgs->object->setGravity(GRAVITY);

	LevelScene* level = new LevelScene(m_Mgs, &m_GameState, &m_GameLoader, m_Settings);
	m_Mgs->scene->add(SceneID::LEVEL, level);

	MenuScene* menu = new MenuScene(m_Mgs, &m_GameState, &m_GameLoader, m_Settings);
	m_Mgs->scene->add(SceneID::MENU, menu);
	m_Mgs->scene->load(SceneID::MENU, true);

	return true;
}

void Game::onUpdate(float a_Dt) {
	m_Mgs->scene->update(a_Dt);
	m_Mgs->ui->update(a_Dt);

	if (m_Mgs->input->getKeyDown(NEW_GAME_KEY)) {
		auto lvlSc = (LevelScene*)m_Mgs->scene->get(SceneID::LEVEL);
		if (m_Mgs->scene->getCurrentSceneIdx() == SceneID::LEVEL) {
			lvlSc->changeLevel(1);
		}
		else {
			if (lvlSc->loadFromFile(1))
				m_Mgs->scene->load(SceneID::LEVEL, false);
		}
	}
}

void Game::onFixedUpdate(float a_FixedDt) {
	m_Mgs->scene->fixedUpdate(a_FixedDt);
}

void Game::onDraw() {
	m_Mgs->scene->draw();
	m_Mgs->ui->draw();

	float wt = m_Mgs->time->getWorldTime();
	float fps = m_Mgs->time->getFPS();

	float log_w = (float)m_Mgs->display->getLogWidth();

	char buff[SHORT_TEXT_MAXLEN];
	snprintf(buff, SHORT_TEXT_MAXLEN, "FPS: %.0f", fps);

	float dy = INFO_FONT.chSize * INFO_FONT.scale;

	m_Mgs->display->drawString({ INFO_POSX, INFO_POSY }, buff, INFO_FONT);

	snprintf(buff, SHORT_TEXT_MAXLEN, "Czas gry: %.1f", wt);
	m_Mgs->display->drawString({ INFO_POSX, INFO_POSY + dy }, buff, INFO_FONT);

	const char* req = "Wymagania: 1234ABCDEFGHIJ";
	float req_size = strlen(req) * INFO_FONT.chSize * INFO_FONT.spacing * INFO_FONT.scale;
	m_Mgs->display->drawString({ log_w - INFO_POSX - req_size,INFO_POSY }, req, INFO_FONT);
}

void Game::onDestroy() {
	
}

void Game::loadUITextures()
{
	m_Mgs->sprite->load(UI_ASSETS "big_frame.bmp", RES::UI_BIG_FRAME);
	m_Mgs->sprite->load(UI_ASSETS "small_frame.bmp", RES::UI_SMALL_FRAME);
	m_Mgs->sprite->load(UI_ASSETS "button.bmp", RES::UI_BUTTON);
	m_Mgs->sprite->load(UI_ASSETS "text_input.bmp", RES::UI_TEXT_INPUT);
	m_Mgs->sprite->load(UI_ASSETS "punch.bmp", RES::PUNCH);
}

void Game::setupBindings()
{
	InputBinds* jump = new InputBinds(1);
	(*jump)[0] = { InputType::KEYBOARD, SDL_SCANCODE_SPACE };
	m_Mgs->input->addBinding(InputBind::JUMP, jump);

	InputBinds* left = new InputBinds(2);
	(*left)[0] = { InputType::KEYBOARD, SDL_SCANCODE_A };
	(*left)[1] = { InputType::KEYBOARD, SDL_SCANCODE_LEFT };
	m_Mgs->input->addBinding(InputBind::LEFT, left);

	InputBinds* right = new InputBinds(2);
	(*right)[0] = { InputType::KEYBOARD, SDL_SCANCODE_D };
	(*right)[1] = { InputType::KEYBOARD, SDL_SCANCODE_RIGHT };
	m_Mgs->input->addBinding(InputBind::RIGHT, right);

	InputBinds* up = new InputBinds(2);
	(*up)[0] = { InputType::KEYBOARD, SDL_SCANCODE_W };
	(*up)[1] = { InputType::KEYBOARD, SDL_SCANCODE_UP };
	m_Mgs->input->addBinding(InputBind::UP, up);

	InputBinds* down = new InputBinds(2);
	(*down)[0] = { InputType::KEYBOARD, SDL_SCANCODE_S };
	(*down)[1] = { InputType::KEYBOARD, SDL_SCANCODE_DOWN };
	m_Mgs->input->addBinding(InputBind::DOWN, down);

	InputBinds* act_x = new InputBinds(3);
	(*act_x)[0] = { InputType::MOUSE, SDL_BUTTON_LEFT };
	(*act_x)[1] = { InputType::KEYBOARD, SDL_SCANCODE_X };
	(*act_x)[2] = { InputType::KEYBOARD, SDL_SCANCODE_K };
	m_Mgs->input->addBinding(InputBind::ACT_X, act_x);

	InputBinds* act_y = new InputBinds(3);
	(*act_y)[0] = { InputType::MOUSE, SDL_BUTTON_RIGHT };
	(*act_y)[1] = { InputType::KEYBOARD, SDL_SCANCODE_C };
	(*act_y)[2] = { InputType::KEYBOARD, SDL_SCANCODE_L };
	m_Mgs->input->addBinding(InputBind::ACT_Y, act_y);
}

void Game::loadCharsets()
{
	m_Mgs->sprite->load(CH_ASSETS "def_8.bmp", RES::CH_8, true);
	m_Mgs->sprite->load(CH_ASSETS "ps2p_16.bmp", RES::CH_16, true);
	m_Mgs->sprite->load(CH_ASSETS "thf_32.bmp", RES::CH_32, true);
	m_Mgs->sprite->load(CH_ASSETS "yg_64.bmp", RES::CH_64, true);
}

void Game::setupStatuses()
{
	m_GameState.addStatus({GODLIKE_THR, GODLIKE_TXT, GODLIKE_MUL});
	m_GameState.addStatus({BRILLIANT_THR, BRILLIANT_TXT, BRILLIANT_MUL});
	m_GameState.addStatus({GREAT_THR, GREAT_TXT, GREAT_MUL});
	m_GameState.addStatus({NICE_THR, NICE_TXT, NICE_MUL});
}
