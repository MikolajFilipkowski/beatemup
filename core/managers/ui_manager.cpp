#include "ui_manager.h"

UIManager::UIManager(Managers* managers) : Manager(managers) {}

UIManager::~UIManager()
{
	destroy();
}

void UIManager::destroy()
{
	for (auto& el : elements) {
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
		el->handleEvents(ev);
	}
}

void UIManager::update(float dt)
{
	for (auto& el : elements) {
		el->update(dt);
	}
}

void UIManager::draw()
{
	for (auto& el : elements) {
		el->draw();
	}
}
