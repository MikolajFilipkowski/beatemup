#pragma once

#include "core_types.h"

struct Font {
	int key;
	int chSize;
	float scale;
	float spacing;
	ColorRGBA color;
};

class UIElement {
protected:
	Vector2 pos;
	FDims size;
	bool active;
	bool focused;
	bool focusable;
	Managers* mgs;
public:
	UIElement(Managers* mgs, Vector2 position, FDims size);
	UIElement(const UIElement&) = delete;
	UIElement& operator=(const UIElement&) = delete;
	virtual ~UIElement() = default;
	virtual void update(float dt) {}
	virtual void handleEvents(SDL_Event& ev) {}
	virtual void draw() = 0;

	Vector2& getPosition() { return pos; }
	FDims& getSize() { return size; }
	void setPosition(Vector2 position) { pos = position; }
	void setSize(FDims size) { this->size = size; }
	bool isActive() const { return active; }
	void setActive(bool ac) { active = ac; }
	bool isFocused() const { return focused; }
	virtual void setFocused(bool fc);

	bool isFocusable() const { return focusable; }
	void setFocusable(bool fc) { focusable = fc; }

	bool isMouseOver(Vector2 mousePos);
};

class UITextElement : public UIElement {
protected:
	char buffer[MAX_TEXTSIZE];
	int maxCharCount;
	Font font;
	ColorRGBA plholder_clr;
	char* plholder_txt;
public:
	UITextElement(Managers* mgs, Vector2 position, FDims size, Font font, int maxChars = 255);
	void setText(const char* text);
	char* getText() const;
	void setFont(Font font);
	Font getFont() const;

	int getCharCount() const;
	int getMaxCharCount() const;

	void setPlaceholder(const char* text, ColorRGBA clr);

	void centerPos(Vector2 parent_pos, FDims parent_size);
	void leftPos(Vector2 parent_pos, FDims parent_size);

	virtual void draw() override;
};

class UIContainer : public UIElement {
protected:
	ArrayList<UIElement*> array;
	int focusedElement;
	
public:
	UIContainer(Managers* mgs, Vector2 position, FDims size) : UIElement(mgs, position, size), focusedElement(-1) {}
	virtual ~UIContainer();
	void addElement(UIElement* el);
	void setFocusedElement(int idx);
	int getFocusedElement() const;

	int getElementCount() const;
	int getFocusableCount() const;

	virtual void update(float dt) override;
	virtual void draw() override;
	virtual void handleEvents(SDL_Event& ev) override;

	void detectScroll(SDL_Event& ev);
	virtual void onScrollEv(int y);

	void detectHover(SDL_Event& ev);
	virtual void onHoverEv(int elIdx);
};

class UIBackgroundContainer : public UIContainer {
protected:
	ColorRGBA bg;
	ColorRGBA border;
	int borderSize;
public:
	UIBackgroundContainer(Managers* mgs, Vector2 position, FDims size) 
		: UIContainer(mgs, position, size), bg(ColorRGBA::transparent()), border(ColorRGBA::transparent()), borderSize(0) {}
	virtual ~UIBackgroundContainer() = default;

	virtual void draw() override;
	void setBackground(ColorRGBA background);
	void setBorder(ColorRGBA color, int size);

	ColorRGBA getBackground();
	ColorRGBA getBorderColor();
	int getBorderSize();
};

class UISpriteBackgroundContainer : public UIBackgroundContainer {
protected:
	int spriteKey;
public:
	UISpriteBackgroundContainer(Managers* mgs, Vector2 position, FDims size)
		: UIBackgroundContainer(mgs, position, size), spriteKey(0) {}
	virtual ~UISpriteBackgroundContainer() = default;

	virtual void draw() override;
	void setSprite(int key);
	int getSprite();
};

class UIButton : public UISpriteBackgroundContainer {
protected:
	UITextElement txt_el;
	bool pressed;
	bool hovered;
	float afterPress;
	void (*onClick)(SDL_Event& ev, UIButton* button, Managers* mgs);
public:
	UIButton(Managers* mgs, Vector2 position, FDims size, Font font, 
		void (*onClick)(SDL_Event& ev, UIButton* button, Managers* mgs) = nullptr);
	virtual void handleEvents(SDL_Event& ev) override;
	virtual void update(float dt) override;
	virtual void draw() override;
	virtual void setText(const char* text);
	
	bool isPressed() const;
	bool isHovered() const;
};

class UITextInput : public UISpriteBackgroundContainer {
protected:
	UITextElement txt_el;
public:
	UITextInput(Managers* mgs, Vector2 position, FDims size, Font font, int maxChars = 255, Vector2 padding = {0,0});
	virtual void draw() override;
	virtual void handleEvents(SDL_Event& ev) override;
	void setPlaceholder(const char* text, ColorRGBA clr);
	virtual void setFocused(bool fc) override;
};