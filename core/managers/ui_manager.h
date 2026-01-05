#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"
#include "../ui.h"

class UIManager : private Manager {
	friend class Engine;
private:
	ArrayList<UIElement*> elements;
	UIManager(Managers* managers);
	~UIManager() override;
	void destroy() override;
public:
	void add(UIElement* el);
	void handleEvents(SDL_Event& ev);
	void update(float dt);
	void draw();
	void clear();
};