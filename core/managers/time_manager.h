#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class TimeManager : private Manager {
	friend class Engine;
private:
	Uint32 m_LastTick{ 0 };
	float m_Dt{ 0 };
	float m_FixedDt{ FIXED_DT };
	float m_Accumulator{ 0 };
	int m_FrameCount{ 0 };
	float m_FpsTimer{ 0 };
	float m_Fps{ 0 };
	float m_WorldTime{ 0 };
	int m_WorldFrame{ 0 };

	TimeManager(Managers* a_Managers);
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

