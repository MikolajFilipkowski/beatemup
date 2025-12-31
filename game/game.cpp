#define _CRT_SECURE_NO_WARNINGS

#include "game.h"

#include <cstdio>

bool Game::onStart(Managers* managers) {
	mgs = managers;
	
	Camera* cam = new Camera(mgs, {0,0,300.0f});
	mgs->display->setActiveCamera(cam);

	mgs->sprite->load("game/assets/player/Idle.bmp", "player_idle");
	mgs->anim->createFromSheet("player_idle", "player_idle", 6, 0.2f);

	mgs->sprite->load("game/assets/def_8.bmp", "charset", true);
	mgs->sprite->load("game/assets/ps2p_16.bmp", "charset_2", true);

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

	sprintf(text, "Szablon drugiego zadania, czas trwania = %.1lf s  %.0lf klatek / s", mgs->time->getWorldTime(), mgs->time->getFPS());
	mgs->display->drawString("charset_2", { (float)((logDims.width / 2) - strlen(text) * 16 / 2), 10 }, text, 1.0f);

	sprintf(text, "Esc - wyjscie, \030 - przyspieszenie, \031 - zwolnienie");
	//mgs->display->drawString("charset", { (float)((logDims.width / 2) - strlen(text) * 12 / 2), 34 }, text, 16);
	mgs->display->drawString("charset", { (float)((logDims.width / 2) - strlen(text) * 16 * 0.67), 18 }, text, 2.0f);

	AnimationClip* idle = mgs->anim->get("player_idle");

	int frame = (int)(mgs->time->getWorldTime() / idle->frameDuration) % idle->frameCount;

	mgs->display->drawAnimFrame("player_idle", frame, { 0, 0, 200 });
}

void Game::onDestroy() {

}
