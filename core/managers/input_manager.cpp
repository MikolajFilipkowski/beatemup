#include "input_manager.h"

#include "managers.h"
#include <stdio.h>

InputManager::InputManager(Managers* a_Managers) : Manager(a_Managers) {
	m_KeyboardState = SDL_GetKeyboardState(&m_NumkeyCount);
	memset(m_PressedNow, 0, m_NumkeyCount);
	memset(m_ReleasedNow, 0, m_NumkeyCount);

	m_MouseState = SDL_GetMouseState(NULL, NULL);
	memset(m_MousePressedNow, 0, MAX_MOUSE_BUTTONS + 1);
	memset(m_MouseReleasedNow, 0, MAX_MOUSE_BUTTONS + 1);
}

InputManager::~InputManager() {
	destroy();
}

void InputManager::destroy() {
	for (auto& pair : m_BindingsMap) {
		delete pair.value;
		pair.value = nullptr;
	}
	m_BindingsMap.clear();
}

void InputManager::updateMouseState()
{
	memset(m_MousePressedNow, 0, MAX_MOUSE_BUTTONS + 1);
	memset(m_MouseReleasedNow, 0, MAX_MOUSE_BUTTONS + 1);

	int rawX, rawY;
	float logX, logY;

	m_MouseState = SDL_GetMouseState(&rawX, &rawY);
	
	Vector2 logStart = m_Mgs->display->getLogStart();
	Vector2 scale = m_Mgs->display->getLogScale();
	Dims dims = m_Mgs->display->getLogDims();

	// Aby pozycja myszy byla niezalezna od wielkosci (i aspect-ratio) ekranu
	logX = (rawX - logStart.x) / scale.x;
	logY = (rawY - logStart.y) / scale.y;

	m_MouseX = clamp(logX, 0.0f, (float)dims.width);
	m_MouseY = clamp(logY, 0.0f, (float)dims.height);
}

bool InputManager::checkActionState(int a_Action, 
	bool (InputManager::*a_KeyFunc)(Uint8) const, bool (InputManager::*a_MouseFunc)(Uint8) const)
{
	if (!m_BindingsMap.containsKey(a_Action)) return false;

	InputBinds*& bdg = m_BindingsMap.get(a_Action);

	if (bdg == nullptr) return false;

	InputBinds& arr = *bdg;

	for (int i = 0; i < arr.count(); i++) {
		if (arr[i].type == InputType::KEYBOARD
			&& (this->*a_KeyFunc)(arr[i].key)
			) {
			return true;
		}
		if (arr[i].type == InputType::MOUSE
			&& (this->*a_MouseFunc)(arr[i].key)
			) {
			return true;
		}
	}
	return false;
}

void InputManager::updateKeyDown(SDL_Event& a_Event)
{
	if (a_Event.key.repeat != 0) return;
	m_PressedNow[a_Event.key.keysym.scancode] = true;
}

void InputManager::updateKeyUp(SDL_Event& a_Event)
{
	if (a_Event.key.repeat != 0) return;
	m_ReleasedNow[a_Event.key.keysym.scancode] = true;
}

void InputManager::updateMouseDown(SDL_Event& a_Event)
{
	Uint8 b = a_Event.button.button;
	if (b > MAX_MOUSE_BUTTONS) return;

	m_MousePressedNow[b] = true;
}

void InputManager::updateMouseUp(SDL_Event& a_Event)
{
	Uint8 b = a_Event.button.button;
	if (b > MAX_MOUSE_BUTTONS) return;

	m_MouseReleasedNow[b] = true;
}

void InputManager::updateState() {
	memset(m_PressedNow, 0, m_NumkeyCount);
	memset(m_ReleasedNow, 0, m_NumkeyCount);

	updateMouseState();
}

bool InputManager::getKey(Uint8 a_Key) const {
	if (a_Key >= m_NumkeyCount) return false;
	return m_KeyboardState[a_Key];
}

bool InputManager::getKeyUp(Uint8 a_Key) const {
	if (a_Key >= m_NumkeyCount) return false;
	return m_ReleasedNow[a_Key];
}

bool InputManager::getKeyDown(Uint8 a_Key) const {
	if (a_Key >= m_NumkeyCount) return false;
	
	return m_PressedNow[a_Key];
}

bool InputManager::getMouseButton(Uint8 a_Button) const
{
	return SDL_BUTTON(a_Button) & m_MouseState;
}

bool InputManager::getMouseButtonDown(Uint8 a_Button) const
{
	if (a_Button > MAX_MOUSE_BUTTONS) return false;
	return m_MousePressedNow[a_Button];
}

bool InputManager::getMouseButtonUp(Uint8 a_Button) const
{
	if (a_Button > MAX_MOUSE_BUTTONS) return false;
	return m_MouseReleasedNow[a_Button];
}

bool InputManager::isMouseOver(Rect a_Rect)
{
	return a_Rect.contains({ (float)m_MouseX, (float)m_MouseY });
}

void InputManager::addBinding(int a_Input, InputBinds*& a_Binding)
{
	m_BindingsMap.put(a_Input, a_Binding);
}

void InputManager::removeBinding(int a_Input)
{
	delete m_BindingsMap.get(a_Input);
	m_BindingsMap.remove(a_Input);
}

bool InputManager::getInput(int a_Input)
{
	return checkActionState(a_Input, &InputManager::getKey, &InputManager::getMouseButton);
}

bool InputManager::getInputDown(int a_Input)
{
	return checkActionState(a_Input, &InputManager::getKeyDown, &InputManager::getMouseButtonDown);
}

bool InputManager::getInputUp(int a_Input)
{
	return checkActionState(a_Input, &InputManager::getKeyUp, &InputManager::getMouseButtonUp);
}

Vector2 InputManager::getMousePos() const
{
	return { (float)m_MouseX, (float)m_MouseY };
}