#include "ui_manager.h"

#include "managers.h"

UIManager::UIManager(Managers* managers) : Manager(managers) {}

UIManager::~UIManager()
{
	destroy();
}

void UIManager::destroy()
{
	for (auto& el : elements) {
		if (el == nullptr) continue;

		delete el;
		el = nullptr;
	}
	elements.clear();
}

void UIManager::add(UIElement* el)
{
	elements.add(el);
}

void UIManager::handleEvents(SDL_Event& ev)
{
	for (auto& el : elements) {
		if (el->isActive())
			el->handleEvents(ev);
	}
}

void UIManager::update(float dt)
{
	for (auto& el : elements) {
		if (el->isActive())
			el->update(dt);
	}
}

void UIManager::draw()
{
	for (auto& el : elements) {
		if (el->isActive())
			el->draw();
	}
}

void UIManager::clear()
{
	destroy();
}

ColorRGBA UIManager::calcPulse(ColorRGBA clr)
{
	ColorRGBA pulse = {};
	float wt = mgs->time->getWorldTime();
	float pulse_effect = fabs(sinf(wt * PULSE_SPD));
	pulse.r = clamp((int)clr.r + (int)(pulse_effect * PULSE_CHANGE), 0, 255);
	pulse.g = clamp((int)clr.g + (int)(pulse_effect * PULSE_CHANGE), 0, 255);
	pulse.b = clamp((int)clr.b + (int)(pulse_effect * PULSE_CHANGE), 0, 255);
	pulse.a = clr.a;

	return pulse;
}

ColorRGBA UIManager::calcAlphaPulse(ColorRGBA clr)
{
	ColorRGBA pulse = {};
	float wt = mgs->time->getWorldTime();
	float pulse_effect = fabs(sinf(wt * PULSE_SPD));
	pulse.r = clr.r;
	pulse.g = clr.g;
	pulse.b = clr.b;
	pulse.a = clamp((int)clr.a - (int)(pulse_effect * PULSE_ALPHA_CHANGE), 0, 255);

	return pulse;
}
