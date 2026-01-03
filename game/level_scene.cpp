#include "level_scene.h"

#include "assert.h"
#include "gameObjects/player.h"

void LevelScene::start()
{
	mgs->sprite->load("game/assets/levels/level1/boxes.bmp", RES::LVL_BOXES);
	mgs->sprite->load("game/assets/levels/level1/buildings.bmp", RES::LVL_BUILDINGS);
	mgs->sprite->load("game/assets/levels/level1/road.bmp", RES::LVL_ROAD);
	mgs->sprite->load("game/assets/levels/level1/sky.bmp", RES::LVL_SKY);
	mgs->sprite->load("game/assets/levels/level1/wall1.bmp", RES::LVL_WALL1);
	mgs->sprite->load("game/assets/levels/level1/wall2.bmp", RES::LVL_WALL2);
	mgs->sprite->load("game/assets/levels/level1/wheels.bmp", RES::LVL_WHEELS);

	addLayer(RES::LVL_SKY, 0.1f, 1280.0f);
	addLayer(RES::LVL_BUILDINGS, 0.2f, 1280.0f);
	addLayer(RES::LVL_WALL2, 0.9f, 1280.0f);
	addLayer(RES::LVL_WALL1, 0.9f, 1280.0f);
	addLayer(RES::LVL_BOXES, 1.0f, 1280.0f);
	addLayer(RES::LVL_WHEELS, 1.0f, 1280.0f);
	addLayer(RES::LVL_ROAD, 1.0f, 1280.0f);

	Camera* cam = new Camera(mgs, { 0,0,500.0f });
	mgs->display->setActiveCamera(cam);


	Transform tr = {
		{0,0,350},
		0.0,
		NO_FLIP,
		{2.0f, 2.0f}
	};
	Player* player = new Player(mgs, cam, tr);
}

void LevelScene::update(float dt)
{
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