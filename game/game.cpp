#define _CRT_SECURE_NO_WARNINGS

#include "game.h"
#include <cstdio>
#include <ctime>
#include <cstdlib>

#define CH_ASSETS "game/assets/charsets/"

Game::Game() : Application() {
	m_InfoFont = {
		RES::CH_16,
		16,
		.75f,
		1.0f,
		ColorRGBA::black()
	};
}

bool Game::onStart(Managers* a_Managers) {
	m_Mgs = a_Managers;
	m_GameLoader.init(m_Mgs);

	srand((unsigned)time(0));

	m_Mgs->display->setIcon("game/assets/punch_icon.bmp");

	setupBindings();
	loadCharsets();

	m_Mgs->sprite->load("game/assets/circle.bmp", RES::CIRCLE);
	m_Mgs->sprite->load("game/assets/shadow.bmp", RES::SHADOW);

	m_GameLoader.loadActionData("game/data/actions.cfg");

	m_Mgs->object->setGravity(GRAVITY);

	LevelScene* level = new LevelScene(m_Mgs);
	m_Mgs->scene->add(SceneID::LEVEL, level);

	MenuScene* menu = new MenuScene(m_Mgs);
	m_Mgs->scene->add(SceneID::MENU, menu);
	m_Mgs->scene->load(SceneID::MENU, true);

	return true;
}

void Game::onUpdate(float a_Dt) {
	//Rect vp = m_Mgs->display->getActiveCamera()->getViewport();
	//printf("%.2f %.2f : %.2f %.2f\n", vp.x, vp.y, vp.x + vp.w, vp.y + vp.h);

	m_Mgs->scene->update(a_Dt);
	m_Mgs->ui->update(a_Dt);

	if (m_Mgs->input->getKeyDown(NEW_GAME_KEY))
		m_Mgs->scene->load(SceneID::LEVEL, false);
}

void Game::onFixedUpdate(float a_FixedDt) {
	m_Mgs->scene->fixedUpdate(a_FixedDt);
}

void Game::onDraw() {
	m_Mgs->scene->draw();
	m_Mgs->ui->draw();

	float wt = m_Mgs->time->getWorldTime();
	float fps = m_Mgs->time->getFPS();
	//size_t length = 0;

	float log_w = (float)m_Mgs->display->getLogWidth();

	char buff[32];
	sprintf_s(buff, 31, "FPS: %.0f", fps);
	//length = strlen(buff);

	float dy = m_InfoFont.chSize * m_InfoFont.scale;

	m_Mgs->display->drawString(RES::CH_16, { 5,3 }, buff, m_InfoFont);

	sprintf_s(buff, 31, "Czas gry: %.1f", wt);
	m_Mgs->display->drawString(RES::CH_16, { 5,3 + dy }, buff, m_InfoFont);

	const char* req = "Wymagania: 1234ABJ";
	float req_size = strlen(req) * m_InfoFont.chSize * m_InfoFont.spacing * m_InfoFont.scale;
	m_Mgs->display->drawString(RES::CH_16, { log_w - 5 - req_size,3 }, req, m_InfoFont);
	

	//sprintf_s(buff, 31, "Czas trwania: %.2f", fps);

	//float posX = f.chSize * f.scale * f.spacing * (float)length;
	//m_Mgs->display->drawString(RES::CH_16, { posX, 0 }, buff, f);

	//Uint64 txtSize = m_Mgs->sprite->loadedSpritesSize();
	//printf("%lf\n", txtSize / (1024.0 * 1024.0));

	/*Dims logDims = m_Mgs->display->getLogDims();
	m_Mgs->display->drawFilledRect({ 4, 4 }, { (float)logDims.width - 8.0f, 64.0f }, ColorRGBA::blue(), ColorRGBA::red());

	char text[128];

	sprintf(text, "Czas trwania = %.1lf s  %.0lf klatek/s", m_Mgs->time->getWorldTime(), m_Mgs->time->getFPS());
	m_Mgs->display->drawString(RES::CH_16, { (float)((logDims.width / 2) - strlen(text) * 24 / 2), 10 }, text, 1.25f);


	sprintf(text, "Esc - wyjscie, F11 - fullscreen");
	m_Mgs->display->drawString(RES::CH_16, { (float)((logDims.width / 2) - strlen(text) * 16 / 2), 40 }, text, 1.0f);*/
}

void Game::onDestroy() {
	
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
