#define _CRT_SECURE_NO_WARNINGS

#include "game.h"

#include <cstdio>

bool Game::onStart(Managers* managers) {
	mgs = managers;
	mgs->sprite->load("cs8x8.bmp", "charset", true);
	return true;
}

void Game::onUpdate(float dt) {

}

void Game::onFixedUpdate(float fixed_dt) {

}

void Game::onDraw() {
	Dims logDims = mgs->display->getLogDims();
	mgs->display->drawFilledRect({ 4, 4 }, { logDims.width - 8, 36 }, ColorRGBA::blue(), ColorRGBA::red());

	char text[128];

	sprintf(text, "Szablon drugiego zadania, czas trwania = %.1lf s  %.0lf klatek / s", mgs->time->getWorldTime(), mgs->time->getFPS());
	mgs->display->drawString("charset", { (float)((logDims.width / 2) - strlen(text) * 8 / 2), 10 }, text);

	sprintf(text, "Esc - wyjscie, \030 - przyspieszenie, \031 - zwolnienie");
	mgs->display->drawString("charset", { (float)((logDims.width / 2) - strlen(text) * 8 / 2), 26 }, text);
}

void Game::onDestroy() {

}
