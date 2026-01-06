#include "ui.h"

#include "core.h"
#include <cstring>
#include <cstdio>


UIElement::UIElement(Managers* mgs, Vector2 position, FDims size)
	: mgs(mgs), pos(position), size(size), active(true), focused(false), focusable(false) {}

void UIElement::setFocused(bool fc) {
	if (focusable)
		focused = fc; 
}

bool UIElement::isMouseOver(Vector2 mousePos)
{
	Rect rect = { pos.x, pos.y, size.width, size.height };
	return rect.contains(mousePos);
}

UITextElement::UITextElement(Managers* mgs, Vector2 position, FDims size, Font font, int maxChars)
	: UIElement(mgs, position, size), font(font), plholder_clr(font.color), plholder_txt(nullptr)
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

int UITextElement::getCharCount() const
{
	return (int)strlen(buffer);
}

int UITextElement::getMaxCharCount() const
{
	return maxCharCount;
}

void UITextElement::setPlaceholder(const char* text, ColorRGBA clr)
{
	plholder_txt = copy_string(text);
	plholder_clr = clr;
}

void UITextElement::centerPos(Vector2 parent_pos, FDims parent_size)
{
	int len = getCharCount();

	const float h = font.chSize * font.scale;
	const float w = len * h * font.spacing;

	const float mid_x = (2 * parent_pos.x + parent_size.width) / 2.0f;
	const float mid_y = (2 * parent_pos.y + parent_size.height) / 2.0f;

	const float x = mid_x - (w / 2);
	const float y = mid_y - font.chSize;

	setSize({ w,h });
	setPosition({ x,y });
}

void UITextElement::leftPos(Vector2 parent_pos, FDims parent_size)
{
	int len = getCharCount();

	const float h = font.chSize * font.scale;
	const float w = len * h * font.spacing;

	const float mid_y = (2 * parent_pos.y + parent_size.height) / 2.0f;

	const float x = parent_pos.x;
	const float y = mid_y - font.chSize;

	setSize({ w,h });
	setPosition({ x,y });
}

void UITextElement::draw()
{
	if (getCharCount() == 0 && !focused) {
		mgs->display->drawString(font.key, pos, plholder_txt, font.scale, font.spacing, size, plholder_clr);
		return;
	}

	float wt = mgs->time->getWorldTime();
	char tempBuffer[MAX_TEXTSIZE];
	strcpy_s(tempBuffer, MAX_TEXTSIZE, buffer);

	if (focused && sinf(wt * 7.0f) > 0) {
		strcat_s(tempBuffer, MAX_TEXTSIZE, "|");
	}

	mgs->display->drawString(font.key, pos, tempBuffer, font.scale, font.spacing, size, font.color);
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

void UIContainer::setFocusedElement(int idx)
{
	for (auto& el : array) {
		el->setFocused(false);
	}
	
	if (idx == -1) return;

	array[idx]->setFocused(true);
	focusedElement = idx;
}

int UIContainer::getFocusedElement() const
{
	return focusedElement;
}

int UIContainer::getElementCount() const
{
	return array.count();
}

int UIContainer::getFocusableCount() const
{
	int counter = 0;
	for (auto& el : array) {
		if (el->isFocusable())
			counter++;
	}
	return counter;
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

	detectScroll(ev);
	detectHover(ev);

	for (auto& el : array) {
		if (el->isActive())
			el->handleEvents(ev);
	}
}

void UIContainer::detectScroll(SDL_Event& ev)
{
	if (!focused && focusable) return;

	if (ev.type == SDL_MOUSEWHEEL) {
		onScrollEv(ev.wheel.y);
	}

	if (mgs->input->getKey(SDL_SCANCODE_DOWN)) {
		onScrollEv(-1);
	}

	if (mgs->input->getKey(SDL_SCANCODE_UP)) {
		onScrollEv(1);
	}
}

void UIContainer::onScrollEv(int y)
{
	int count = getElementCount();
	if (count <= 1) return;

	int cur = getFocusedElement();
	if (cur < 0) cur = 0;

	int sign = (y > 0) ? -1 : 1;

	// Zmienia focused element na kolejny w danym kierunku
	int next = cur;
	for (int i = 0; i < count; i++) {
		next += sign + count;
		next %= count;

		if (array[next]->isFocusable())
			break;
	}

	setFocusedElement(next);
}

void UIContainer::detectHover(SDL_Event& ev)
{
	if (!mgs->display->cursorShown()) return;

	if (ev.type == SDL_MOUSEMOTION) {
		Vector2 mousePos = { (float)ev.motion.x, (float)ev.motion.y };

		for (int i = 0; i < array.count(); i++) {
			Vector2& pos = array[i]->getPosition();
			FDims& size = array[i]->getSize();

			Rect rect = {
				pos.x,
				pos.y,
				size.width,
				size.height
			};

			if (rect.contains(mousePos)) {
				onHoverEv(i);
			}
		}
	}
}

void UIContainer::onHoverEv(int elIdx) {
	if (array[elIdx]->isFocusable()) {
		setFocusedElement(elIdx);
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
	focusable = true;
	afterPress = 0.0f;

	txt_el.centerPos(pos, size);
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
		txt_el.centerPos(pos, size);
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
		txt_el.centerPos(pos, size);
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
			txt_el.centerPos(pos, size);
			afterPress = 0.0f;
		}
	}

	if (pressed) {
		if (afterPress == 0.0f) {
			pos.x += 2;
			pos.y += 3;
			txt_el.centerPos(pos, size);
		}
		
		pressed = false;
		afterPress = BTN_PR_DUR;
	}
}

void UIButton::draw()
{
	if (!active) return;

	ColorRGBA dp_bg = bg;
	ColorRGBA dp_border = border;

	if (focused) {
		dp_bg = mgs->ui->calcPulse(bg);
		dp_border = mgs->ui->calcAlphaPulse(border);
	}

	mgs->display->drawFilledRect(pos, size, dp_bg, dp_border, borderSize);

	if (spriteKey != 0)
		mgs->display->drawSprite(spriteKey, pos, size);

	for (auto& el : array) {
		if (el->isActive())
			el->draw();
	}

	txt_el.draw();
}

void UIButton::setText(const char* text)
{
	txt_el.setText(text);
	txt_el.centerPos(pos, size);
}

bool UIButton::isPressed() const
{
	return pressed;
}

bool UIButton::isHovered() const
{
	return hovered;
}

UITextInput::UITextInput(Managers* mgs, Vector2 position, FDims size, Font font, int maxChars, Vector2 padding)
	: UISpriteBackgroundContainer(mgs, position, size), 
	txt_el(mgs, position, size, font, maxChars) 
{
	focusable = true;
	txt_el.setFocusable(true);
	txt_el.leftPos({pos.x + padding.x, pos.y + padding.y}, {size.width - 2 * padding.x, size.height - 2 * padding.y});
}

void UITextInput::draw()
{
	if (!active) return;

	mgs->display->drawFilledRect(pos, size, bg, border, borderSize);

	if (spriteKey != 0)
		mgs->display->drawSprite(spriteKey, pos, size);

	txt_el.draw();
}

void UITextInput::handleEvents(SDL_Event& ev)
{
	if (!focused || !active) return;

	char* txt = txt_el.getText();
	int charCount = txt_el.getCharCount();
	int maxCharCount = txt_el.getMaxCharCount();

	if (ev.type == SDL_KEYDOWN) {
		if (ev.key.keysym.sym == SDLK_BACKSPACE && charCount > 0) {
			txt[--charCount] = '\0';
		}
	}
	else if (ev.type == SDL_TEXTINPUT) {

		char first = ev.text.text[0];
		if (first < 32) return;

		if (charCount < maxCharCount) {
			strcat_s(txt, (rsize_t)maxCharCount + 1, ev.text.text);
		}
	}
}

void UITextInput::setPlaceholder(const char* text, ColorRGBA clr)
{
	txt_el.setPlaceholder(text, clr);
}

void UITextInput::setFocused(bool fc)
{
	if (focusable) {
		focused = fc;
		txt_el.setFocused(fc);
	}
}
