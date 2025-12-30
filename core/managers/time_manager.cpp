#include "time_manager.h"

#include "managers.h"

TimeManager::TimeManager(Managers* managers) : Manager(managers),
	lastTick(0), dt(0.0f), fixedDt(FIXED_DT), accumulator(0.0f),
	frameCount(0), fpsTimer(0.0f), fps(0.0f), worldTime(0.0f) {
}

TimeManager::~TimeManager() {}

void TimeManager::destroy() {}

void TimeManager::tick()
{
	Uint32 frameTime = SDL_GetTicks() - lastTick;

	//Ograniczenie fps
	if (frameTime < MIN_FRAMETIME) {
		SDL_Delay(MIN_FRAMETIME - frameTime);
	}

	//Przy zawieszeniu pauza
	if (frameTime > MAX_FRAMETIME) {
		lastTick = SDL_GetTicks();
	}

	Uint32 currentTick = SDL_GetTicks();
	dt = (currentTick - lastTick) * 0.001f;

	lastTick = currentTick;

	worldTime += dt;

	fpsTimer += dt;
	frameCount++;

	if (fpsTimer > FPS_INTERVAL) {
		fps = frameCount / fpsTimer;
		frameCount = 0;
		fpsTimer -= FPS_INTERVAL;
	};

	if (dt > MAX_DT)
		dt = MAX_DT;

	accumulator += dt;
}

bool TimeManager::step()
{
	if (accumulator >= fixedDt) {
		accumulator -= fixedDt;
		return true;
	}
	return false;
}