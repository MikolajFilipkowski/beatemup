#pragma once

#include "managers/managers.h"
#include "application.h"

extern "C" {
#include"./SDL2/include/SDL.h"
}

class Engine {
private:
	bool running;
	Managers mgs;
	Application* app;
public:
	Engine();

	bool init(const char* title, Dims screenDims, Dims logicalDims = {0,0}, bool isFullscreen = false);
	void run(Application* application);
	void handleEvents();
	void stop();
	void destroy();

	bool isRunning() const;
};