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
	int worldFrame;

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
	float getAccum() const;
	float getIFactor() const;
	int getWorldFrame() const;
	void resetWorldVars();
};

