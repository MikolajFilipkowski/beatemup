#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

#include "core/core.h"
#include "game/game.h"

#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	Engine engine{};
	GameLoader gameLoader{engine.getMgs()};
	GameSettings settings{};
	Game game{ gameLoader, settings };

	bool loaded = gameLoader.loadGameSettings("game/data/settings.cfg", settings);
	if (!loaded) return 1;

	engine.init("Beat 'em up!", 
		{
			(int)settings.windowWidth, 
			(int)settings.windowHeight
		}, 
		{1280, 720}, 
		false
	);
	engine.run(&game);

	return 0;
}