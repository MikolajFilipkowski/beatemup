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
	Game game{};
	engine.init("Beat 'em up!", {1280, 720}, {0,0}, false);
	engine.run(&game);

	return 0;
}