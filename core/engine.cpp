#define _CRT_SECURE_NO_WARNINGS

#include "engine.h"

#include <cstdio>
#include <cstdarg>

Engine::Engine() {
	m_Mgs.engine = this;
}

bool Engine::init(const char* a_Title, Dims a_ScreenDims, Dims a_LogDims, bool a_Fullscreen)
{
	m_Mgs.display = new DisplayManager(&m_Mgs);
	m_Mgs.sprite = new SpriteManager(&m_Mgs);
	m_Mgs.object = new ObjectManager(&m_Mgs);
	m_Mgs.time = new TimeManager(&m_Mgs);
	m_Mgs.input = new InputManager(&m_Mgs);
	m_Mgs.anim = new AnimationManager(&m_Mgs);
	m_Mgs.scene = new SceneManager(&m_Mgs);
	m_Mgs.ui = new UIManager(&m_Mgs);
	m_Mgs.engine = this;

	bool displayInitialized = m_Mgs.display->init(a_Title, a_ScreenDims, a_LogDims, a_Fullscreen);

	if (!displayInitialized) {
		return false;
	}

	m_Running = true;

	return true;
}

void Engine::run(Application* a_App)
{
	if (a_App == nullptr) return;
	m_App = a_App;

	m_Mgs.time->m_LastTick = SDL_GetTicks();

	bool appInitialized = m_App->onStart(&m_Mgs);
	if (!appInitialized) stop();

	while (m_Running) {
		m_Mgs.time->tick();

		m_Mgs.input->updateState();
		handleEvents();

		while (m_Mgs.time->step()) {
			m_App->onFixedUpdate(m_Mgs.time->getFixedDt());
		}

		m_App->onUpdate(m_Mgs.time->getDt());

		m_Mgs.object->refreshObjects();

		m_Mgs.display->clear();

		m_App->onDraw();

		m_Mgs.display->present();
	}

	destroy();
}

void Engine::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			m_Mgs.input->updateKeyDown(event);
			if (event.key.keysym.scancode == EXIT_KEY && m_App->onEscPressed()) 
				stop();
			if (event.key.keysym.scancode == FULLSCREEN_KEY && !m_App->m_FullscreenDisabled)
				m_Mgs.display->toggleFullscreen();
			if (event.key.keysym.scancode == DEBUG_KEY)
				m_DebugMode = !m_DebugMode;
			break;
		case SDL_KEYUP:
			m_Mgs.input->updateKeyUp(event);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_Mgs.input->updateMouseDown(event);
			break;
		case SDL_MOUSEBUTTONUP:
			m_Mgs.input->updateMouseUp(event);
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) 
				m_Mgs.display->updateScreenDims();
			break;
		case SDL_QUIT:
			stop();
			break;
		};

		m_Mgs.ui->handleEvents(event);
	};
}

void Engine::stop()
{
	m_Running = false;
}

void Engine::destroy()
{
	if (m_App != nullptr) {
		m_App->onDestroy();
	}

	for (int i = m_Mgs.m_ArrLen - 1; i >= 0; i--) {
		if (*m_Mgs.m_Array[i]) {
			(*m_Mgs.m_Array[i])->destroy();
			delete (*m_Mgs.m_Array[i]);
			*m_Mgs.m_Array[i] = nullptr;
		}
	}
}

void Engine::throwError(const char* a_Msg, ...)
{
	va_list args;
	va_start(args, a_Msg);

	char buff[MAX_ERR_LEN]{};

	vsnprintf(buff, MAX_ERR_LEN, a_Msg, args);
	fprintf(stderr, buff);
	if (m_Mgs.display == nullptr) return;

	SDL_Window* win = m_Mgs.display->m_Window;
	if (win == nullptr) return;

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", buff, win);

	stop();
	va_end(args);
}

bool Engine::inDebugMode() const
{
	return m_DebugMode;
}

void Engine::setDebugMode(bool a_Debug)
{
	m_DebugMode = a_Debug;
}

bool Engine::isRunning() const
{
	return m_Running;
}