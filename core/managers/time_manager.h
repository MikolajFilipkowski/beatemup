#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class TimeManager : private Manager {
	friend class Engine;
private:
	Uint32 lastTick;
	float dt;
	float fixedDt;
	float accumulator;
	int frameCount;
	float fpsTimer;
	float fps;
	float worldTime;

	TimeManager(Managers* managers);
	~TimeManager() override;
	void destroy() override;
public:
	void tick();
	bool step();
	float getDt() const;
	float getFixedDt() const;
	float getFPS() const;
	float getWorldTime() const;
	void setWorldTime(float wt);
};

inline float TimeManager::getDt() const
{
	return dt;
}

inline float TimeManager::getFixedDt() const
{
	return fixedDt;
}

inline float TimeManager::getFPS() const {
	return fps;
}

inline float TimeManager::getWorldTime() const
{
	return worldTime;
}

inline void TimeManager::setWorldTime(float wt)
{
	worldTime = wt;
}


