#include "level_scene.h"

#include "assert.h"
#include "../gameObjects/player.h"
#include "../gameObjects/player_camera.h"

#define MENU_ASSETS "game/assets/levels/menu/"

void LevelScene::start()
{
	mgs->display->showCursor(false);
	iBuffer->clear();

	mgs->sprite->load(MENU_ASSETS "boxes.bmp", RES::MENU_BOXES);
	mgs->sprite->load(MENU_ASSETS "buildings.bmp", RES::MENU_BUILDINGS);
	mgs->sprite->load(MENU_ASSETS "road.bmp", RES::MENU_ROAD);
	mgs->sprite->load(MENU_ASSETS "sky.bmp", RES::MENU_SKY);
	mgs->sprite->load(MENU_ASSETS "wall1.bmp", RES::MENU_WALL1);
	mgs->sprite->load(MENU_ASSETS "wall2.bmp", RES::MENU_WALL2);
	mgs->sprite->load(MENU_ASSETS "wheels.bmp", RES::MENU_WHEELS);

	addLayer(RES::MENU_SKY, 0.1f, 1280.0f, 720.0f);
	addLayer(RES::MENU_BUILDINGS, 0.2f, 1280.0f, 720.0f);
	addLayer(RES::MENU_WALL2, 1.0f, 1280.0f, 720.0f);
	addLayer(RES::MENU_WALL1, 1.0f, 1280.0f, 720.0f);
	addLayer(RES::MENU_BOXES, 1.0f, 1280.0f, 720.0f);
	addLayer(RES::MENU_WHEELS, 1.0f, 1280.0f, 720.0f);
	addLayer(RES::MENU_ROAD, 1.0f, 2000.0f, 720.f);

	Transform tr = {
		{0,0,350},
		0.0,
		NO_FLIP,
		{2.5f, 2.5f}
	};
	Player* ply = new Player(mgs, iBuffer, tr);

	PlayerCamera* cam = new PlayerCamera(mgs, ply, { 0,0,500.0f });
	mgs->display->setActiveCamera((Camera*)cam);
}

void LevelScene::update(float dt)
{
	iBuffer->update();
	mgs->object->updateAll(dt);
}

void LevelScene::fixedUpdate(float fixed_dt)
{
	mgs->object->fixedUpdateAll(fixed_dt);
}

void LevelScene::draw()
{
	drawBackground();
	mgs->object->drawAll();
}

void LevelScene::destroy()
{
	mgs->object->clear();
}

void LevelScene::loadFromFile(const char* fileName)
{
	assert(mgs->scene->getCurrentScene() == this && "Scene should be loaded before being displayed!");
}