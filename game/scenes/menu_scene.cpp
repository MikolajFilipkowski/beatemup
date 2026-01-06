#include "menu_scene.h"

#include <cstring>
#include <cstdio>

#define MENU_ASSETS "game/assets/levels/menu/"

void MenuScene::start()
{
	mgs->display->showCursor(true);

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

	const ColorRGBA t_color = { 0x1E, 0x1E, 0x1E, 0xFF };

	mgs->sprite->load(ASSETS "punch.bmp", RES::PUNCH_ICON);
	mgs->sprite->setSpriteColor(RES::PUNCH_ICON, t_color);

	Dims d = mgs->display->getLogDims();

	menu = new UIBackgroundContainer(
		mgs,
		{ d.width * .25f, d.height * .15f },
		{ d.width * .5f, d.height * .7f }
	);

	Font t_font = {
		RES::CH_32,
		32,
		1.5f,
		.75f,
		t_color
	};

	Font input_font = {
		RES::CH_16,
		16,
		2.0f,
		1.0f,
		t_color
	};

	const float ch_size = t_font.chSize * t_font.scale;

	const char* t_text = "Beat 'em up";

	const float t_size = (strlen(t_text)) * ch_size * t_font.spacing;

	const float t_x = (d.width / 2.0f) - (t_size / 2.0f);
	const float t_y = d.height * .18f;

	UISpriteBackgroundContainer* punch = new UISpriteBackgroundContainer(
		mgs, { t_x + IC_OFF_X, t_y + IC_OFF_Y }, { IC_SIZE, IC_SIZE }
	);
	punch->setSprite(RES::PUNCH_ICON);
	
	UITextElement* title = new UITextElement(
		mgs,
		{ t_x, t_y },
		{ t_size, ch_size },
		t_font
	);

	title->setText(t_text);


	auto onClick = [](SDL_Event& ev, UIButton* button, Managers* mgs) -> void {
		mgs->scene->load(SceneID::LEVEL, false);
	};

	auto exit = [](SDL_Event& ev, UIButton* button, Managers* mgs) -> void {
		mgs->engine->stop();
		};

	UIButton* btn_1 = new UIButton(
		mgs,
		{ d.width * .35f, d.height * .45f },
		{ d.width * .3f, MENU_BTN_H },
		t_font,
		onClick
	);

	UIButton* btn_2 = new UIButton(
		mgs,
		{ d.width * .35f, d.height * .45f  + MENU_BTN_H + MENU_BTN_GAP},
		{ d.width * .3f, MENU_BTN_H },
		t_font
	);

	UIButton* btn_3 = new UIButton(
		mgs,
		{ d.width * .35f, d.height * .45f + 2 * MENU_BTN_H + 2 * MENU_BTN_GAP },
		{ d.width * .3f, MENU_BTN_H },
		t_font,
		exit
	);

	UITextInput* txt_input = new UITextInput(
		mgs,
		{ d.width * .35f, d.height * .45f - MENU_BTN_H - MENU_BTN_GAP },
		{ d.width * .3f, MENU_BTN_H },
		input_font,
		10,
		{25, 0}
	);

	txt_input->setBackground(ColorRGBA::white());
	txt_input->setBorder(ColorRGBA::black(), 3);
	txt_input->setPlaceholder("Pseudonim", { 0x77, 0x77, 0x77, 0xFF });

	btn_1->setBackground(ColorRGBA::blue());
	btn_1->setBorder(ColorRGBA::black(), 3);
	btn_1->setText("Graj");

	btn_2->setBackground(ColorRGBA::blue());
	btn_2->setBorder(ColorRGBA::black(), 3);
	btn_2->setText("Wyniki");

	btn_3->setBackground(ColorRGBA::blue());
	btn_3->setBorder(ColorRGBA::black(), 3);
	btn_3->setText("Wyjscie");

	menu->setBackground({0x66, 0xAA, 0x66, 0xFF});
	menu->setBorder({0x0F, 0x0F, 0x0F, 0xFF}, 8);

	menu->addElement((UIElement*)punch);
	menu->addElement((UIElement*)title);
	menu->addElement((UIElement*)txt_input);
	menu->addElement((UIElement*)btn_1);
	menu->addElement((UIElement*)btn_2);
	menu->addElement((UIElement*)btn_3);

	mgs->ui->add((UIElement*)menu);

	cam = new Camera(mgs);
	mgs->display->setActiveCamera(cam);
}

void MenuScene::update(float dt)
{
	if (cam == nullptr) return;
	Vector3 pos = cam->getPosition();
	cam->setPosition({pos.x + .25f, pos.y, pos.z});
}

void MenuScene::fixedUpdate(float fixed_dt)
{
}

void MenuScene::draw()
{
	drawBackground();
}

void MenuScene::destroy()
{
	if (mgs->ui) mgs->ui->clear();

	mgs->sprite->unload(RES::MENU_BOXES);
	mgs->sprite->unload(RES::MENU_BUILDINGS);
	mgs->sprite->unload(RES::MENU_ROAD);
	mgs->sprite->unload(RES::MENU_SKY);
	mgs->sprite->unload(RES::MENU_WALL1);
	mgs->sprite->unload(RES::MENU_WALL2);
	mgs->sprite->unload(RES::MENU_WHEELS);
}
