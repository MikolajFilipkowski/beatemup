#define _CRT_SECURE_NO_WARNINGS

#include "game.h"
#include <cstdio>

#define CH_ASSETS "game/assets/charsets/"

Game::Game() : Application(), gmLoader(nullptr), iBuffer(nullptr), debugMode(false) {
	infoFont = {
		RES::CH_16,
		16,
		.75f,
		1.0f,
		ColorRGBA::black()
	};
}

bool Game::onStart(Managers* managers) {
	mgs = managers;

	mgs->display->setIcon("game/assets/punch_icon.bmp");

	setupBindings();
	loadCharsets();

	mgs->sprite->load("game/assets/circle.bmp", RES::CIRCLE);
	mgs->sprite->load("game/assets/shadow.bmp", RES::SHADOW);

	gmLoader = new GameLoader(mgs);
	gmLoader->loadActionData("game/data/actions.cfg");

	iBuffer = new InputBuffer(mgs);

	LevelScene* level = new LevelScene(mgs, iBuffer);
	mgs->scene->add(SceneID::LEVEL, level);

	MenuScene* menu = new MenuScene(mgs);
	mgs->scene->add(SceneID::MENU, menu);
	mgs->scene->load(SceneID::MENU, true);

	return true;
}

void Game::onUpdate(float dt) {
	//Rect vp = mgs->display->getActiveCamera()->getViewport();
	//printf("%.2f %.2f : %.2f %.2f\n", vp.x, vp.y, vp.x + vp.w, vp.y + vp.h);

	mgs->scene->update(dt);
	mgs->ui->update(dt);

	if (mgs->input->getKeyDown(DEBUG_KEY))
		debugMode = !debugMode;
	if (mgs->input->getKeyDown(NEW_GAME_KEY))
		mgs->scene->load(SceneID::LEVEL, false);
}

void Game::onFixedUpdate(float fixed_dt) {
	mgs->scene->fixedUpdate(fixed_dt);
}

void Game::onDraw() {
	mgs->scene->draw();
	mgs->ui->draw();

	float wt = mgs->time->getWorldTime();
	float fps = mgs->time->getFPS();
	//size_t length = 0;

	float log_w = (float)mgs->display->getLogWidth();

	char buff[32];
	sprintf_s(buff, 31, "FPS: %.0f", fps);
	//length = strlen(buff);

	float dy = infoFont.chSize * infoFont.scale;

	mgs->display->drawString(RES::CH_16, { 5,3 }, buff, infoFont);

	sprintf_s(buff, 31, "Czas gry: %.1f", wt);
	mgs->display->drawString(RES::CH_16, { 5,3 + dy }, buff, infoFont);

	const char* req = "Wymagania: 1234A";
	float req_size = strlen(req) * infoFont.chSize * infoFont.spacing * infoFont.scale;
	mgs->display->drawString(RES::CH_16, { log_w - 5 - req_size,3 }, req, infoFont);

	if (inDebugMode()) 
		iBuffer->drawBuffer();
	

	//sprintf_s(buff, 31, "Czas trwania: %.2f", fps);

	//float posX = f.chSize * f.scale * f.spacing * (float)length;
	//mgs->display->drawString(RES::CH_16, { posX, 0 }, buff, f);

	//Uint64 txtSize = mgs->sprite->loadedSpritesSize();
	//printf("%lf\n", txtSize / (1024.0 * 1024.0));

	/*Dims logDims = mgs->display->getLogDims();
	mgs->display->drawFilledRect({ 4, 4 }, { (float)logDims.width - 8.0f, 64.0f }, ColorRGBA::blue(), ColorRGBA::red());

	char text[128];

	sprintf(text, "Czas trwania = %.1lf s  %.0lf klatek/s", mgs->time->getWorldTime(), mgs->time->getFPS());
	mgs->display->drawString(RES::CH_16, { (float)((logDims.width / 2) - strlen(text) * 24 / 2), 10 }, text, 1.25f);


	sprintf(text, "Esc - wyjscie, F11 - fullscreen");
	mgs->display->drawString(RES::CH_16, { (float)((logDims.width / 2) - strlen(text) * 16 / 2), 40 }, text, 1.0f);*/
}

void Game::onDestroy() {
	delete gmLoader;
	delete iBuffer;

	gmLoader = nullptr;
	iBuffer = nullptr;
}

bool Game::inDebugMode() const
{
	return debugMode;
}

void Game::setDebugMode(bool d)
{
	debugMode = d;
}

void Game::setupBindings()
{
	ActionBinding* jump = new ActionBinding(1);
	(*jump)[0] = { InputType::KEYBOARD, SDL_SCANCODE_SPACE };
	mgs->input->addBinding(ActionBind::JUMP, jump);

	ActionBinding* left = new ActionBinding(2);
	(*left)[0] = { InputType::KEYBOARD, SDL_SCANCODE_A };
	(*left)[1] = { InputType::KEYBOARD, SDL_SCANCODE_LEFT };
	mgs->input->addBinding(ActionBind::LEFT, left);

	ActionBinding* right = new ActionBinding(2);
	(*right)[0] = { InputType::KEYBOARD, SDL_SCANCODE_D };
	(*right)[1] = { InputType::KEYBOARD, SDL_SCANCODE_RIGHT };
	mgs->input->addBinding(ActionBind::RIGHT, right);

	ActionBinding* up = new ActionBinding(2);
	(*up)[0] = { InputType::KEYBOARD, SDL_SCANCODE_W };
	(*up)[1] = { InputType::KEYBOARD, SDL_SCANCODE_UP };
	mgs->input->addBinding(ActionBind::UP, up);

	ActionBinding* down = new ActionBinding(2);
	(*down)[0] = { InputType::KEYBOARD, SDL_SCANCODE_S };
	(*down)[1] = { InputType::KEYBOARD, SDL_SCANCODE_DOWN };
	mgs->input->addBinding(ActionBind::DOWN, down);

	ActionBinding* act_x = new ActionBinding(3);
	(*act_x)[0] = { InputType::MOUSE, SDL_BUTTON_LEFT };
	(*act_x)[1] = { InputType::KEYBOARD, SDL_SCANCODE_X };
	(*act_x)[2] = { InputType::KEYBOARD, SDL_SCANCODE_K };
	mgs->input->addBinding(ActionBind::ACT_X, act_x);

	ActionBinding* act_y = new ActionBinding(3);
	(*act_y)[0] = { InputType::MOUSE, SDL_BUTTON_RIGHT };
	(*act_y)[1] = { InputType::KEYBOARD, SDL_SCANCODE_C };
	(*act_y)[2] = { InputType::KEYBOARD, SDL_SCANCODE_L };
	mgs->input->addBinding(ActionBind::ACT_Y, act_y);
}

void Game::loadCharsets()
{
	mgs->sprite->load(CH_ASSETS "def_8.bmp", RES::CH_8, true);
	mgs->sprite->load(CH_ASSETS "ps2p_16.bmp", RES::CH_16, true);
	mgs->sprite->load(CH_ASSETS "thf_32.bmp", RES::CH_32, true);
	mgs->sprite->load(CH_ASSETS "yg_64.bmp", RES::CH_64, true);
}
