#include "input_manager.h"

#include "managers.h"
#include <stdio.h>

InputManager::InputManager(Managers* managers) : Manager(managers) {
	keyboardState = SDL_GetKeyboardState(&numkeyCount);
	prevKeyboardState = new Uint8[numkeyCount];
	memset(prevKeyboardState, 0, numkeyCount);

	prevMouseState = 0;
	mouseState = SDL_GetMouseState(NULL, NULL);
}

InputManager::~InputManager() {
	destroy();
}

void InputManager::destroy() {
	delete[] prevKeyboardState;
	prevKeyboardState = nullptr;
}

void InputManager::updateMouseState()
{
	int rawX, rawY;
	float logX, logY;

	mouseState = SDL_GetMouseState(&rawX, &rawY);
	
	Vector2 logStart = mgs->display->getLogStart();
	Vector2 scale = mgs->display->getLogScale();
	Dims dims = mgs->display->getLogDims();

	// Aby pozycja myszy byla niezalezna od wielkosci (i aspect-ratio) ekranu
	logX = (rawX - logStart.x) / scale.x;
	logY = (rawY - logStart.y) / scale.y;

	mouseX = clamp(logX, 0.0f, (float)dims.width);
	mouseY = clamp(logY, 0.0f, (float)dims.height);
}

void InputManager::updateState() {
	memcpy(prevKeyboardState, keyboardState, numkeyCount);

	prevMouseState = mouseState;
	updateMouseState();
}

bool InputManager::getKey(Uint8 key) const {
	if (key >= numkeyCount) return false;
	return keyboardState[key];
}

bool InputManager::getKeyUp(Uint8 key) const {
	if (key >= numkeyCount) return false;
	return !keyboardState[key] && prevKeyboardState[key];
}

bool InputManager::getKeyDown(Uint8 key) const {
	if (key >= numkeyCount) return false;
	return keyboardState[key] && !prevKeyboardState[key];
}

bool InputManager::getMouseButton(Uint8 button) const
{
	return SDL_BUTTON(button) & mouseState;
}

bool InputManager::getMouseButtonDown(Uint8 button) const
{
	return (SDL_BUTTON(button) & mouseState) && !(SDL_BUTTON(button) & prevMouseState);
}

bool InputManager::isMouseOver(Rect rect)
{
	return rect.contains({ (float)mouseX, (float)mouseY });
}

Vector2 InputManager::getMousePos() const
{
	return { (float)mouseX, (float)mouseY };
}