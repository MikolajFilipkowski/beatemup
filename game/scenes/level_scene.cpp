#include "level_scene.h"

#include "assert.h"
#include "../gameObjects/player.h"


#define MENU_ASSETS "game/assets/levels/menu/"

void LevelScene::start()
{
	m_Mgs->display->showCursor(false);

	m_Mgs->sprite->load(MENU_ASSETS "boxes.bmp", RES::MENU_BOXES);
	m_Mgs->sprite->load(MENU_ASSETS "buildings.bmp", RES::MENU_BUILDINGS);
	m_Mgs->sprite->load(MENU_ASSETS "road.bmp", RES::MENU_ROAD);
	m_Mgs->sprite->load(MENU_ASSETS "sky.bmp", RES::MENU_SKY);
	m_Mgs->sprite->load(MENU_ASSETS "wall1.bmp", RES::MENU_WALL1);
	m_Mgs->sprite->load(MENU_ASSETS "wall2.bmp", RES::MENU_WALL2);
	m_Mgs->sprite->load(MENU_ASSETS "wheels.bmp", RES::MENU_WHEELS);

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

	m_Player = new Player(m_Mgs, tr);

	m_Camera = new PlayerCamera(m_Mgs, m_Player, { 0,0,500.0f });
	m_Mgs->display->setActiveCamera((Camera*)m_Camera);
}

void LevelScene::update(float a_Dt)
{
	m_Mgs->object->updateAll(a_Dt);
}

void LevelScene::fixedUpdate(float a_FixedDt)
{
	m_Mgs->object->fixedUpdateAll(a_FixedDt);
}

void LevelScene::draw()
{
	drawBackground();
	m_Mgs->object->drawAll();

	if (m_Mgs->engine->inDebugMode()) {
		m_Player->getIBuffer().drawBuffer();
		m_Player->drawActionName();
	}	
}

void LevelScene::destroy()
{
	m_Mgs->object->clear();
}

void LevelScene::loadFromFile(const char* a_FileName)
{
	assert(m_Mgs->scene->getCurrentScene() == this && "Scene should be loaded before being displayed!");
}