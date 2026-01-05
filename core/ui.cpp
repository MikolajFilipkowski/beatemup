#include "ui.h"

#include "core.h"
#include <cstring>


UIElement::UIElement(Managers* mgs, Vector2 position, FDims size)
	: mgs(mgs), pos(position), size(size), active(true), focused(false) {
}

bool UIElement::isMouseOver(Vector2 mousePos)
{
	Rect rect = { pos.x, pos.y, size.width, size.height };
	return rect.contains(mousePos);
}

UITextElement::UITextElement(Managers* mgs, Vector2 position, FDims size, Font font, int maxChars)
	: UIElement(mgs, position, size), charCount(0), font(font)
{
	maxCharCount = (maxChars >= MAX_TEXTSIZE) ? MAX_TEXTSIZE - 1 : maxChars;
	memset(buffer, 0, MAX_TEXTSIZE);
}

void UITextElement::setText(const char* text)
{
	strcpy_s(buffer, maxCharCount, text);
	buffer[maxCharCount] = '\0';
}

char* UITextElement::getText() const {
	return (char*)buffer;
}

void UITextElement::setFont(Font font)
{
	this->font = font;
}

Font UITextElement::getFont() const
{
	return font;
}

void UITextElement::draw()
{
	mgs->display->drawString(font.key, pos, buffer, font.scale, font.spacing, size, font.color);
}

UITextInput::UITextInput(Managers* mgs, Vector2 position, FDims size, Font font, int maxChars)
	: UITextElement(mgs, position, size, font, maxChars) {}

void UITextInput::draw()
{
	mgs->display->drawString(font.key, pos, buffer, font.scale, font.spacing, size, font.color);
}

void UITextInput::handleEvents(SDL_Event& ev)
{
	if (ev.type == SDL_KEYDOWN) {
		if (ev.key.keysym.sym == SDLK_BACKSPACE && charCount > 0) {
			buffer[--charCount] = '\0';
		}
	}
	else if (ev.type == SDL_TEXTINPUT) {
		if (charCount < maxCharCount) {
			strcat_s(buffer, MAX_TEXTSIZE, ev.text.text);
			charCount = (int)strlen(buffer);
		}
	}
}

UIContainer::~UIContainer()
{
	for (auto& el : array) {
		if (el == nullptr) continue;

		delete el;
		el = nullptr;
	}
	array.clear();
}

void UIContainer::addElement(UIElement* el)
{
	array.add(el);
}

void UIContainer::update(float dt)
{
	if (!active) return;
	for (auto& el : array) {
		if (el->isActive())
			el->update(dt);
	}
}

void UIContainer::draw()
{
	if (!active) return;
	for (auto& el : array) {
		if (el->isActive())
			el->draw();
	}
}

void UIContainer::handleEvents(SDL_Event& ev)
{
	if (!active) return;
	for (auto& el : array) {
		if (el->isActive())
			el->handleEvents(ev);
	}
}

void UIBackgroundContainer::draw()
{
	if (!active) return;

	mgs->display->drawFilledRect(pos, size, bg, border, borderSize);

	for (auto& el : array) {
		if (el->isActive())
			el->draw();
	}
}

void UIBackgroundContainer::setBackground(ColorRGBA background)
{
	bg = background;
}

void UIBackgroundContainer::setBorder(ColorRGBA color, int size)
{
	border = color;
	borderSize = size;
}

ColorRGBA UIBackgroundContainer::getBackground()
{
	return bg;
}

ColorRGBA UIBackgroundContainer::getBorderColor()
{
	return border;
}

int UIBackgroundContainer::getBorderSize()
{
	return borderSize;
}

void UISpriteBackgroundContainer::draw()
{
	if (!active) return;

	mgs->display->drawFilledRect(pos, size, bg, border, borderSize);

	if (spriteKey != 0)
		mgs->display->drawSprite(spriteKey, pos, size);

	for (auto& el : array) {
		if (el->isActive())
			el->draw();
	}
}

void UISpriteBackgroundContainer::setSprite(int sprite_key)
{
	spriteKey = sprite_key;
}

int UISpriteBackgroundContainer::getSprite()
{
	return spriteKey;
}

UIButton::UIButton(Managers* mgs, Vector2 position, FDims size, Font font, void (*onClick)(SDL_Event& ev, UIButton* button, Managers* mgs))
	: UISpriteBackgroundContainer(mgs, position, size), txt_el(mgs, position, size, font), onClick(onClick)
{
	pressed = false;
	hovered = false;
	afterPress = 0.0f;

	changeText();
}

void UIButton::handleEvents(SDL_Event& ev)
{
	Rect rect = { pos.x, pos.y, size.width, size.height };

	if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_RETURN) {
		if (focused) {
			if (onClick != nullptr)
				onClick(ev, this, mgs);
			pressed = true;
			return;
		}
	}
	if (ev.type == SDL_KEYUP && ev.key.keysym.sym == SDLK_RETURN) {
		pressed = false;
		changeText();
	}

	if (!mgs->display->cursorShown()) return;

	if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT) {
		float posX = (float)ev.button.x;
		float posY = (float)ev.button.y;

		if (rect.contains({ posX, posY })) {
			if (onClick != nullptr)
				onClick(ev, this, mgs);
			pressed = true;
			return;
		}
	}

	if (ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_LEFT) {
		pressed = false;
		changeText();
	}
	
	if (ev.type == SDL_MOUSEMOTION) {
		float posX = (float)ev.motion.x;
		float posY = (float)ev.motion.y;

		if (rect.contains({ posX, posY })) {
			hovered = true;
		}
		else {
			hovered = false;
		}
	}
}

void UIButton::update(float dt)
{
	if (afterPress > 0.0f) {
		afterPress -= dt;

		if (afterPress <= 0.0f) {
			pos.x -= 2;
			pos.y -= 3;
			changeText();
			afterPress = 0.0f;
		}
	}

	if (pressed) {
		if (afterPress == 0.0f) {
			pos.x += 2;
			pos.y += 3;
			changeText();
		}
		
		pressed = false;
		afterPress = BTN_PR_DUR;
	}
}

void UIButton::draw()
{
	if (!active) return;

	ColorRGBA dp_border = border;

	if (focused) {
		float wt = mgs->time->getWorldTime();
		float pulse_effect = fabs(sinf(wt * BTN_PULSE_SPD));
		dp_border.a = 255 * pulse_effect;
	}

	mgs->display->drawFilledRect(pos, size, bg, dp_border, borderSize);

	if (spriteKey != 0)
		mgs->display->drawSprite(spriteKey, pos, size);

	for (auto& el : array) {
		if (el->isActive())
			el->draw();
	}

	txt_el.draw();
}

void UIButton::changeText()
{
	int len = (int)strlen(txt_el.getText());
	Font f = txt_el.getFont();

	const float h = f.chSize * f.scale;
	const float w = len * h * f.spacing;

	const float mid_x = (2 * pos.x + size.width) / 2.0f;
	const float mid_y = (2 * pos.y + size.height) / 2.0f;

	const float x = mid_x - (w / 2);
	const float y = mid_y - f.chSize;

	txt_el.setSize({ w,h });
	txt_el.setPosition({ x,y });
}

void UIButton::setText(const char* text)
{
	txt_el.setText(text);
	changeText();
}

bool UIButton::isPressed() const
{
	return pressed;
}

bool UIButton::isHovered() const
{
	return hovered;
}