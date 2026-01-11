#include "time_manager.h"

#include "managers.h"

TimeManager::TimeManager(Managers* a_Managers) : Manager(a_Managers) {}

TimeManager::~TimeManager() {}

void TimeManager::destroy() {}

void TimeManager::tick()
{
	Uint32 frameTime = SDL_GetTicks() - m_LastTick;

	//Ograniczenie fps
	if (frameTime < MIN_FRAMETIME) {
		SDL_Delay(MIN_FRAMETIME - frameTime);
	}

	//Przy zawieszeniu pauza
	if (frameTime > MAX_FRAMETIME) {
		m_LastTick = SDL_GetTicks();
	}

	Uint32 currentTick = SDL_GetTicks();
	m_Dt = (currentTick - m_LastTick) * 0.001f;

	m_LastTick = currentTick;

	m_WorldTime += m_Dt;

	m_FpsTimer += m_Dt;
	m_FrameCount++;

	if (m_FpsTimer > FPS_INTERVAL) {
		m_Fps = m_FrameCount / m_FpsTimer;
		m_FrameCount = 0;
		m_FpsTimer -= FPS_INTERVAL;
	};

	if (m_Dt > MAX_DT)
		m_Dt = MAX_DT;

	m_Accumulator += m_Dt;
}

bool TimeManager::step()
{
	if (m_Accumulator >= m_FixedDt) {
		m_WorldFrame++;
		m_Accumulator -= m_FixedDt;
		return true;
	}
	return false;
}

float TimeManager::getDt() const
{
	return m_Dt;
}

float TimeManager::getFixedDt() const
{
	return m_FixedDt;
}

float TimeManager::getFPS() const {
	return m_Fps;
}

float TimeManager::getWorldTime() const
{
	return m_WorldTime;
}

float TimeManager::getAccum() const
{
	return m_Accumulator;
}

float TimeManager::getIFactor() const
{
	return clamp(m_Accumulator / m_FixedDt, 0.0f, 1.0f);
}

int TimeManager::getWorldFrame() const
{
	return m_WorldFrame;
}

void TimeManager::resetWorldVars()
{
	m_WorldFrame = 0;
	m_WorldTime = 0.0f;
}
