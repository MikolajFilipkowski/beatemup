#pragma once

#include "core_types.h"

struct Font {
	int key;
	float scale;
	float spacing;
};

class UIElement {
protected:
	Vector2 pos;
	FDims size;
	bool active;
	bool focused;
	Managers* mgs;
public:
	UIElement(Managers* mgs, Vector2 position, FDims size);
	virtual ~UIElement() = default;
	virtual void update(float dt) {}
	virtual void handleEvents(SDL_Event& ev) {}
	virtual void draw() = 0;

	Vector2& getPosition() { return pos; }
	FDims& getSize() { return size; }
	bool isActive() const { return active; }
	void setActive(bool ac) { active = ac; }
	bool isFocused() const { return focused; }
	void setFocused(bool fc) { focused = fc; }
};

class UITextElement : public UIElement {
protected:
	char buffer[MAX_TEXTSIZE];
	int charCount;
	int maxCharCount;
	Font font;
public:
	UITextElement(Managers* mgs, Vector2 position, FDims size, int maxChars, Font font);
	virtual void draw() override;
};

class UITextInput : public UITextElement {
public:
	UITextInput(Managers* mgs, Vector2 position, FDims size, int maxChars, Font font);
	virtual void draw() override;
	virtual void handleEvents(SDL_Event& ev) override;
};

class UIButton : public UITextElement {
public:
	UIButton(Managers* mgs, Vector2 position, FDims size, int maxChars, Font font);
	virtual void draw() override;
};