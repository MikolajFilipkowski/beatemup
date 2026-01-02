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

	ActionBinding* up = new ActionBinding(2);
	(*up)[0] = { InputType::KEYBOARD, SDL_SCANCODE_W };
	(*up)[1] = { InputType::KEYBOARD, SDL_SCANCODE_UP };

	ActionBinding* down = new ActionBinding(2);
	(*down)[0] = { InputType::KEYBOARD, SDL_SCANCODE_S };
	(*down)[1] = { InputType::KEYBOARD, SDL_SCANCODE_DOWN };

	mgs->input->addBinding(Action::JUMP, jump);
	mgs->input->addBinding(Action::LEFT, left);
	mgs->input->addBinding(Action::RIGHT, right);
	mgs->input->addBinding(Action::UP, up);
	mgs->input->addBinding(Action::DOWN, down);

	Camera* cam = new Camera(mgs, {0,0,500.0f});
	mgs->display->setActiveCamera(cam);

	mgs->sprite->load("game/assets/charsets/def_8.bmp", RES::CH_8, true);
	mgs->sprite->load("game/assets/charsets/ps2p_16.bmp", RES::CH_16, true);
	mgs->sprite->load("game/assets/charsets/thf_32.bmp", RES::CH_32, true);
	mgs->sprite->load("game/assets/charsets/yg_64.bmp", RES::CH_64, true);

	mgs->sprite->load("game/assets/City1.bmp", RES::BG);

	Transform tr_bg = {
		{0,370,0},
		0.0,
		NO_FLIP,
		{1.0f, 1.0f}
	};

	bg = new SpriteObject(mgs, tr_bg, RES::BG);

	Transform tr = {
		{0,0,350},
		0.0,
		NO_FLIP,
		{1.5f, 1.5f}
	};
	player = new Player(mgs, cam, tr);

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
	mgs->display->drawString(RES::CH_64, { (float)((logDims.width / 2) - strlen(text) * 8 / 2), 10 }, text, 0.5f, 0.7f);
	

	sprintf(text, "Esc - wyjscie, F11 - fullscreen");
	mgs->display->drawString(RES::CH_16, { (float)((logDims.width / 2) - strlen(text) * 16 / 2), 40 }, text, 1.0f);

}

void Game::onDestroy() {

}
