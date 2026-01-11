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
	ArrayList<UIElement*> m_Elements{};
	UIManager(Managers* a_Managers);
	~UIManager() override;
	void destroy() override;
public:
	void add(UIElement* a_Element);
	void remove(UIElement* a_Element);
	void handleEvents(SDL_Event& a_Event);
	void update(float a_Dt);
	void draw();
	void clear();

	ColorRGBA calcPulse(ColorRGBA a_Color);
	ColorRGBA calcAlphaPulse(ColorRGBA a_Color);
};