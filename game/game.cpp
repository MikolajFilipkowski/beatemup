#define _CRT_SECURE_NO_WARNINGS

#include "game.h"
#include <cstdio>

#define CH_ASSETS "game/assets/charsets/"

Game::Game() : Application() {}

bool Game::onStart(Managers* managers) {
	mgs = managers;

	mgs->display->setIcon("game/assets/punch_icon.bmp");

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

	ActionBinding* act_x = new ActionBinding(3);
	(*act_x)[0] = { InputType::MOUSE, SDL_BUTTON_LEFT };
	(*act_x)[1] = { InputType::KEYBOARD, SDL_SCANCODE_X };
	(*act_x)[2] = { InputType::KEYBOARD, SDL_SCANCODE_K };

	ActionBinding* act_y = new ActionBinding(3);
	(*act_y)[0] = { InputType::MOUSE, SDL_BUTTON_RIGHT };
	(*act_y)[1] = { InputType::KEYBOARD, SDL_SCANCODE_C };
	(*act_y)[2] = { InputType::KEYBOARD, SDL_SCANCODE_L };

	mgs->input->addBinding(Action::JUMP, jump);
	mgs->input->addBinding(Action::LEFT, left);
	mgs->input->addBinding(Action::RIGHT, right);
	mgs->input->addBinding(Action::UP, up);
	mgs->input->addBinding(Action::DOWN, down);
	mgs->input->addBinding(Action::ACT_X, act_x);
	mgs->input->addBinding(Action::ACT_Y, act_y);

	mgs->sprite->load(CH_ASSETS "def_8.bmp", RES::CH_8, true);
	mgs->sprite->load(CH_ASSETS "ps2p_16.bmp", RES::CH_16, true);
	mgs->sprite->load(CH_ASSETS "thf_32.bmp", RES::CH_32, true);
	mgs->sprite->load(CH_ASSETS "yg_64.bmp", RES::CH_64, true);

	mgs->sprite->load("game/assets/shadow.bmp", RES::SHADOW);

	LevelScene* level = new LevelScene(mgs);
	mgs->scene->add(SceneID::LEVEL, level);
	//mgs->scene->load(SceneID::LEVEL);

	MenuScene* menu = new MenuScene(mgs);
	mgs->scene->add(SceneID::MENU, menu);
	mgs->scene->load(SceneID::MENU, true);

	return true;
}

void Game::onUpdate(float dt) {
	Rect vp = mgs->display->getActiveCamera()->getViewport();
	//printf("%.2f %.2f : %.2f %.2f\n", vp.x, vp.y, vp.x + vp.w, vp.y + vp.h);

	mgs->scene->update(dt);
	mgs->ui->update(dt);
}

void Game::onFixedUpdate(float fixed_dt) {
	mgs->scene->fixedUpdate(fixed_dt);
}

void Game::onDraw() {
	mgs->scene->draw();
	mgs->ui->draw();

	/*Dims logDims = mgs->display->getLogDims();
	mgs->display->drawFilledRect({ 4, 4 }, { (float)logDims.width - 8.0f, 64.0f }, ColorRGBA::blue(), ColorRGBA::red());

	char text[128];

	sprintf(text, "Czas trwania = %.1lf s  %.0lf klatek/s", mgs->time->getWorldTime(), mgs->time->getFPS());
	mgs->display->drawString(RES::CH_16, { (float)((logDims.width / 2) - strlen(text) * 24 / 2), 10 }, text, 1.25f);


	sprintf(text, "Esc - wyjscie, F11 - fullscreen");
	mgs->display->drawString(RES::CH_16, { (float)((logDims.width / 2) - strlen(text) * 16 / 2), 40 }, text, 1.0f);*/
}

void Game::onDestroy() {
	
}
