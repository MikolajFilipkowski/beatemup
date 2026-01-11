#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

static constexpr size_t MAX_MOUSE_BUTTONS = 12;

class InputManager : private Manager {
	friend class Engine;
private:
	Map<int, InputBinds*> m_BindingsMap{};
	const Uint8* m_KeyboardState;
	bool m_PressedNow[SDL_NUM_SCANCODES]{};
	bool m_ReleasedNow[SDL_NUM_SCANCODES]{};

	Uint32 m_MouseState{0};
	bool m_MousePressedNow[MAX_MOUSE_BUTTONS + 1]{};
	bool m_MouseReleasedNow[MAX_MOUSE_BUTTONS + 1]{};

	int m_NumkeyCount{ 0 };
	float m_MouseX{ 0 };
	float m_MouseY{ 0 };

	InputManager(Managers* a_Managers);
	~InputManager() override;
	void destroy() override;
	void updateMouseState();
	// \brief Przyjmuje akcje i dwie funkcje (do obslugi klawiatury i myszy)
	bool checkActionState(int a_Action, 
		bool (InputManager::*a_KeyFunc)(Uint8) const, bool (InputManager::*a_MouseFunc)(Uint8) const);
public:
	void updateKeyDown(SDL_Event& a_Event);
	void updateKeyUp(SDL_Event& a_Event);
	void updateMouseDown(SDL_Event& a_Event);
	void updateMouseUp(SDL_Event& a_Event);
	void updateState();
	bool getKey(Uint8 a_Key) const;
	bool getKeyUp(Uint8 a_Key) const;
	bool getKeyDown(Uint8 a_Key) const;
	bool getMouseButton(Uint8 a_Button) const;
	bool getMouseButtonDown(Uint8 a_Button) const;
	bool getMouseButtonUp(Uint8 a_Button) const;
	bool isMouseOver(Rect a_Rect);

	void addBinding(int a_Input, InputBinds*& a_Binding);
	void removeBinding(int a_Input);
	bool getInput(int a_Input);
	bool getInputDown(int a_Input);
	bool getInputUp(int a_Input);

	Vector2 getMousePos() const;
};