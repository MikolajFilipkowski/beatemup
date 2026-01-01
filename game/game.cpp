#define _CRT_SECURE_NO_WARNINGS

#include "game.h"
#include "gameObjects/player.h"

#include <cstdio>

Game::Game() : Application(), player(nullptr) {}

bool Game::onStart(Managers* managers) {
	mgs = managers;

	ActionBinding* jump = new ActionBinding(1);
	(*jump)[0] = { InputType::KEYBOARD, SDL_SCANCODE_SPACE };

	ActionBinding* left = new ActionBinding(2);
	(*left)[0] = { InputType::KEYBOARD, SDL_SCANCODE_A };
	(*left)[1] = { InputType::KEYBOARD, SDL_SCANCODE_LEFT };

	ActionBinding* right = new ActionBinding(2);
	(*right)[0] = { InputType::KEYBOARD, SDL_SCANCODE_D };
	(*right)[1] = { InputType::KEYBOARD, SDL_SCANCODE_RIGHT };

	mgs->input->addBinding("jump", jump);
	mgs->input->addBinding("left", left);
	mgs->input->addBinding("right", right);

	Camera* cam = new Camera(mgs, {0,0,300.0f});
	mgs->display->setActiveCamera(cam);

	mgs->sprite->load("game/assets/charsets/def_8.bmp", "ch_8", true);
	mgs->sprite->load("game/assets/charsets/ps2p_16.bmp", "ch_16", true);
	mgs->sprite->load("game/assets/charsets/thf_32.bmp", "ch_32", true);
	mgs->sprite->load("game/assets/charsets/yg_64.bmp", "ch_64", true);

	Transform tr = {
		{0,0,200},
		0.0,
		NO_FLIP,
		{1.5f, 1.5f}
	};
	player = new Player(mgs, tr);

	return true;
}

void Game::onUpdate(float dt) {

}

void Game::onFixedUpdate(float fixed_dt) {
	
}

void Game::onDraw() {
	Dims logDims = mgs->display->getLogDims();
	mgs->display->drawFilledRect({ 4, 4 }, { logDims.width - 8, 64 }, ColorRGBA::blue(), ColorRGBA::red());

	char text[128];

	sprintf(text, "czas trwania = %.1lf s  %.0lf klatek/s", mgs->time->getWorldTime(), mgs->time->getFPS());
	mgs->display->drawString("ch_64", { (float)((logDims.width / 2) - strlen(text) * 8 / 2), 10 }, text, 0.5f, 0.7f);
	

	sprintf(text, "Esc - wyjscie, F11 - fullscreen");
	mgs->display->drawString("ch_16", { (float)((logDims.width / 2) - strlen(text) * 16 / 2), 40 }, text, 1.0f);

	sprintf(text, "Beat 'em up");
	mgs->display->drawString("ch_64", { (float)((logDims.width / 2) - strlen(text) * 128 / 2), 50 }, text, 2.0f, 0.7f);
}

void Game::onDestroy() {

}
