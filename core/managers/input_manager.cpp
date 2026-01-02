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
	for (auto& pair : bindings) {
		delete pair.value;
		pair.value = nullptr;
	}
	destroy();
}

void InputManager::destroy() {
	delete[] prevKeyboardState;
	prevKeyboardState = nullptr;
}

void InputManager::updateMouseState()
{
	prevMouseState = mouseState;

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

bool InputManager::checkActionState(int action, 
	bool (InputManager::*keyFun)(Uint8) const, bool (InputManager::*mouseFun)(Uint8) const)
{
	if (!bindings.containsKey(action)) return false;

	ActionBinding*& bdg = bindings.get(action);

	if (bdg == nullptr) return false;

	ActionBinding& arr = *bdg;

	for (int i = 0; i < arr.count(); i++) {
		if (arr[i].type == InputType::KEYBOARD
			&& (this->*keyFun)(arr[i].key)
			) {
			return true;
		}
		if (arr[i].type == InputType::MOUSE
			&& (this->*mouseFun)(arr[i].key)
			) {
			return true;
		}
	}
	return false;
}

void InputManager::updateState() {
	memcpy(prevKeyboardState, keyboardState, numkeyCount);
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

bool InputManager::getMouseButtonUp(Uint8 button) const
{
	return !(SDL_BUTTON(button) & mouseState) && (SDL_BUTTON(button) & prevMouseState);
}

bool InputManager::isMouseOver(Rect rect)
{
	return rect.contains({ (float)mouseX, (float)mouseY });
}

void InputManager::addBinding(int action, ActionBinding*& bdg)
{
	bindings.put(action, bdg);
}

void InputManager::removeBinding(int action)
{
	delete bindings.get(action);
	bindings.remove(action);
}

bool InputManager::getAction(int action)
{
	return checkActionState(action, &InputManager::getKey, &InputManager::getMouseButton);
}

bool InputManager::getActionDown(int action)
{
	return checkActionState(action, &InputManager::getKeyDown, &InputManager::getMouseButtonDown);
}

bool InputManager::getActionUp(int action)
{
	return checkActionState(action, &InputManager::getKeyUp, &InputManager::getMouseButtonUp);
}

Vector2 InputManager::getMousePos() const
{
	return { (float)mouseX, (float)mouseY };
}