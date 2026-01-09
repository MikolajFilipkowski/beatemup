#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

static constexpr int MAX_MOUSE_BUTTONS = 12;

class InputManager : private Manager {
	friend class Engine;
private:
	Map<int, ActionBinding*> bindings;
	Uint8 const* keyboardState;
	bool* pressedNow;
	bool* releasedNow;

	Uint32 mouseState;
	bool* mousePressedNow;
	bool* mouseReleasedNow;

	int numkeyCount;
	float mouseX;
	float mouseY;

	InputManager(Managers* managers);
	~InputManager() override;
	void destroy() override;
	void updateMouseState();
	// \brief Przyjmuje akcje i dwie funkcje (do obslugi klawiatury i myszy)
	bool checkActionState(int action, 
		bool (InputManager::*keyFun)(Uint8) const, bool (InputManager::*mouseFun)(Uint8) const);
public:
	void updateKeyDown(SDL_Event& ev);
	void updateKeyUp(SDL_Event& ev);
	void updateMouseDown(SDL_Event& ev);
	void updateMouseUp(SDL_Event& ev);
	void updateState();
	bool getKey(Uint8 key) const;
	bool getKeyUp(Uint8 key) const;
	bool getKeyDown(Uint8 key) const;
	bool getMouseButton(Uint8 button) const;
	bool getMouseButtonDown(Uint8 button) const;
	bool getMouseButtonUp(Uint8 button) const;
	bool isMouseOver(Rect rect);

	void addBinding(int action, ActionBinding*& bdg);
	void removeBinding(int action);
	bool getAction(int action);
	bool getActionDown(int action);
	bool getActionUp(int action);

	Vector2 getMousePos() const;
};