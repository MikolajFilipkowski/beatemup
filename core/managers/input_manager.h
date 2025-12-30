#pragma once

extern "C" {
#include"../SDL2/include/SDL.h"
}

#include "manager.h"
#include "../core_types.h"

class InputManager : private Manager {
	friend class Engine;
private:
	Uint8 const* keyboardState;
	Uint8* prevKeyboardState;
	Uint32 mouseState;
	Uint32 prevMouseState;
	int numkeyCount;
	float mouseX;
	float mouseY;

	InputManager(Managers* managers);
	~InputManager() override;
	void destroy() override;
	void updateMouseState();
public:
	void updateState();
	bool getKey(Uint8 key) const;
	bool getKeyUp(Uint8 key) const;
	bool getKeyDown(Uint8 key) const;
	bool getMouseButton(Uint8 button) const;
	bool getMouseButtonDown(Uint8 button) const;
	bool isMouseOver(Rect rect);

	Vector2 getMousePos() const;
};