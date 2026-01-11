#pragma once

#include "managers/managers.h"
#include "application.h"

extern "C" {
#include"./SDL2/include/SDL.h"
}

class Engine {
private:
	bool m_Running{ false };
	Managers m_Mgs{};
	Application* m_App{};
	bool m_DebugMode{ false };
public:
	Engine();

	bool init(const char* a_Title, Dims a_ScreenDims, Dims a_LogDims = {0,0}, bool a_Fullscreen = false);
	void run(Application* a_App);
	void handleEvents();
	void stop();
	void destroy();

	bool inDebugMode() const;
	void setDebugMode(bool a_DebugMode);

	bool isRunning() const;
};