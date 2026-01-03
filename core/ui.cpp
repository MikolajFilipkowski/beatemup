#include "ui.h"

#include "core.h"
#include <cstring>

UIElement::UIElement(Managers* mgs, Vector2 position, FDims size)
	: mgs(mgs), pos(position), size(size), active(false), focused(false) {
}

UITextElement::UITextElement(Managers* mgs, Vector2 position, FDims size, int maxChars, Font font)
	: UIElement(mgs, position, size), charCount(0), font(font)
{
	maxCharCount = (maxChars >= MAX_TEXTSIZE) ? MAX_TEXTSIZE - 1 : maxChars;
	memset(buffer, 0, MAX_TEXTSIZE);
}

void UITextElement::draw()
{
	mgs->display->drawString(font.key, pos, buffer, font.scale, font.spacing);
}

UITextInput::UITextInput(Managers* mgs, Vector2 position, FDims size, int maxChars, Font font)
	: UITextElement(mgs, position, size, maxChars, font) {}

void UITextInput::draw()
{
	mgs->display->drawString(font.key, pos, buffer, font.scale, font.spacing);
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

UIButton::UIButton(Managers* mgs, Vector2 position, FDims size, int maxChars, Font font)
	: UITextElement(mgs, position, size, maxChars, font) {}

void UIButton::draw()
{
	mgs->display->drawString(font.key, pos, buffer, font.scale, font.spacing);
}