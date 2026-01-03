#define _CRT_SECURE_NO_WARNINGS

#include "engine.h"

#include <stdio.h>

Engine::Engine() : running(false), mgs{}, app(nullptr) {
	mgs.engine = this;
}

bool Engine::init(const char* title, Dims screenDims, Dims logicalDims, bool isFullscreen)
{
	mgs.display = new DisplayManager(&mgs);
	mgs.sprite = new SpriteManager(&mgs);
	mgs.object = new ObjectManager(&mgs);
	mgs.time = new TimeManager(&mgs);
	mgs.input = new InputManager(&mgs);
	mgs.anim = new AnimationManager(&mgs);
	mgs.scene = new SceneManager(&mgs);
	mgs.ui = new UIManager(&mgs);
	mgs.engine = this;

	bool displayInitialized = mgs.display->init(title, screenDims, logicalDims, isFullscreen);

	if (!displayInitialized) {
		return false;
	}

	running = true;

	return true;
}

void Engine::run(Application* application)
{
	if (application == nullptr) return;
	app = application;

	mgs.time->lastTick = SDL_GetTicks();

	bool appInitialized = app->onStart(&mgs);
	if (!appInitialized) stop();

	while (running) {
		mgs.time->tick();

		mgs.input->updateState();
		handleEvents();

		while (mgs.time->step()) {
			app->onFixedUpdate(mgs.time->getFixedDt());
		}

		app->onUpdate(mgs.time->getDt());

		mgs.object->refreshObjects();

		mgs.display->clear();

		app->onDraw();

		mgs.display->present();
	}

	destroy();
}

void Engine::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE && app->onEscPressed()) 
				stop();
			if (event.key.keysym.sym == SDLK_F11 && !app->fullscreenDisabled)
				mgs.display->toggleFullscreen();
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) 
				mgs.display->updateScreenDims();
			break;
		case SDL_QUIT:
			stop();
			break;
		};

		mgs.ui->handleEvents(event);
	};
}

void Engine::stop()
{
	running = false;
}

void Engine::destroy()
{
	if (app != nullptr) {
		app->onDestroy();
	}

	for (int i = mgs.arrLen - 1; i >= 0; i--) {
		if (*mgs.array[i]) {
			(*mgs.array[i])->destroy();
			delete (*mgs.array[i]);
			*mgs.array[i] = nullptr;
		}
	}
}

bool Engine::isRunning() const
{
	return running;
}