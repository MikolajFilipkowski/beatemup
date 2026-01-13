#include "level_scene.h"

#include "assert.h"
#include "../gameObjects/player.h"
#include "../gameObjects/doyle.h"
#include "../gameObjects/autumn.h"

#define MENU_ASSETS "game/assets/levels/menu/"
#define UI_ASSETS "game/assets/ui/"

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

	m_Mgs->sprite->load(UI_ASSETS "hb_frame.bmp", RES::HB_FRAME);
	m_Mgs->sprite->load(UI_ASSETS "hb_fill.bmp", RES::HB_FILL);
	m_Mgs->sprite->load(UI_ASSETS "hb_ghost.bmp", RES::HB_GHOST);

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

	Transform doyleTr = {
		{300,0,350},
		0.0,
		H_FLIP,
		{2.5f, 2.5f}
	};

	Transform autumnTr = {
		{-300,0,350},
		0.0,
		NO_FLIP,
		{2.5f, 2.7f}
	};

	auto doyle = new Doyle(m_Mgs, (Actor*)&m_Player, doyleTr);
	auto autumn = new Autumn(m_Mgs, (Actor*)&m_Player, autumnTr);

	m_Healthbar = new UIHealthbar(m_Mgs, { 5, 50 }, { 300, 45}, {19, 0});
	m_Healthbar->setMax(m_Player->getHP());
	m_Healthbar->linkVals(&m_Player->getHP());
	m_Healthbar->setSprite(RES::HB_FRAME);
	m_Healthbar->setFill(RES::HB_FILL);
	m_Healthbar->setGhost(RES::HB_GHOST);
	m_Mgs->ui->add((UIElement*)m_Healthbar);
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

		for (auto& obj : m_Mgs->object->getAllObjects()) {
			if (!(obj->getType() & ObjectType::ACTOR)) continue;

			Actor* actor = (Actor*)obj;
			actor->drawCollBoxes();
		}
	}	
}

void LevelScene::destroy()
{
	m_Mgs->object->clear();

	if (m_Mgs->ui) m_Mgs->ui->clear();
	m_Healthbar = nullptr;

	m_Mgs->sprite->unload(RES::MENU_BOXES);
	m_Mgs->sprite->unload(RES::MENU_BUILDINGS);
	m_Mgs->sprite->unload(RES::MENU_ROAD);
	m_Mgs->sprite->unload(RES::MENU_SKY);
	m_Mgs->sprite->unload(RES::MENU_WALL1);
	m_Mgs->sprite->unload(RES::MENU_WALL2);
	m_Mgs->sprite->unload(RES::MENU_WHEELS);

	m_Mgs->sprite->unload(RES::HB_FRAME);
	m_Mgs->sprite->unload(RES::HB_FILL);
	m_Mgs->sprite->unload(RES::HB_GHOST);
}

void LevelScene::loadFromFile(const char* a_FileName)
{
	assert(m_Mgs->scene->getCurrentScene() == this && "Scene should be loaded before being displayed!");
}