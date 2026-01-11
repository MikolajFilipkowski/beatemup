#include "ui_manager.h"

#include "managers.h"

UIManager::UIManager(Managers* a_Managers) : Manager(a_Managers) {}

UIManager::~UIManager()
{
	destroy();
}

void UIManager::destroy()
{
	for (auto& el : m_Elements) {
		if (el == nullptr) continue;

		delete el;
		el = nullptr;
	}
	m_Elements.clear();
}

void UIManager::add(UIElement* a_Element)
{
	m_Elements.add(a_Element);
}

void UIManager::remove(UIElement* a_Element)
{
	m_Elements.remove(a_Element);
	delete a_Element;
}

void UIManager::handleEvents(SDL_Event& a_Event)
{
	for (auto& el : m_Elements) {
		if (el != nullptr && el->isActive())
			el->handleEvents(a_Event);
	}
}

void UIManager::update(float a_Dt)
{
	for (auto& el : m_Elements) {
		if (el != nullptr && el->isActive())
			el->update(a_Dt);
	}
}

void UIManager::draw()
{
	for (auto& el : m_Elements) {
		if (el != nullptr && el->isActive())
			el->draw();
	}
}

void UIManager::clear()
{
	destroy();
}

ColorRGBA UIManager::calcPulse(ColorRGBA a_Color)
{
	ColorRGBA pulse = {};
	float wt = m_Mgs->time->getWorldTime();
	float pulseEffect = fabs(sinf(wt * PULSE_SPD));
	pulse.r = clamp((int)a_Color.r + (int)(pulseEffect * PULSE_CHANGE), 0, 0xFF);
	pulse.g = clamp((int)a_Color.g + (int)(pulseEffect * PULSE_CHANGE), 0, 0xFF);
	pulse.b = clamp((int)a_Color.b + (int)(pulseEffect * PULSE_CHANGE), 0, 0xFF);
	pulse.a = a_Color.a;

	return pulse;
}

ColorRGBA UIManager::calcAlphaPulse(ColorRGBA a_Color)
{
	ColorRGBA pulse = {};
	float wt = m_Mgs->time->getWorldTime();
	float pulseEffect = fabs(sinf(wt * PULSE_SPD));
	pulse.r = a_Color.r;
	pulse.g = a_Color.g;
	pulse.b = a_Color.b;
	pulse.a = clamp((int)a_Color.a - (int)(pulseEffect * PULSE_ALPHA_CHANGE), 0, 0xFF);

	return pulse;
}
