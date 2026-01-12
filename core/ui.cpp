#include "ui.h"

#include "core.h"
#include <cstring>
#include <cstdio>


UIElement::UIElement(Managers* m_Mgs, Vector2 a_Pos, FDims a_Size)
	: m_Mgs(m_Mgs), m_Pos(a_Pos), m_Size(a_Size) {}

void UIElement::setFocused(bool a_Focused) {
	if (m_Focusable)
		m_Focused = a_Focused; 
}

bool UIElement::isMouseOver(Vector2 a_MousePos)
{
	Rect rect = { m_Pos.x, m_Pos.y, m_Size.width, m_Size.height };
	return rect.contains(a_MousePos);
}

UITextElement::UITextElement(Managers* m_Mgs, Vector2 a_Pos, FDims a_Size, Font a_Font, int a_MaxChars)
	: UIElement(m_Mgs, a_Pos, a_Size), m_Font(a_Font), m_PlaceholderFont(a_Font)
{
	m_MaxCharCount = (a_MaxChars >= MAX_TEXTSIZE) ? MAX_TEXTSIZE - 1 : a_MaxChars;
	memset(m_Buffer, 0, MAX_TEXTSIZE);
}

void UITextElement::setText(const char* a_Text)
{
	strcpy_s(m_Buffer, m_MaxCharCount, a_Text);
	m_Buffer[m_MaxCharCount] = '\0';
}

char* UITextElement::getText() const {
	return (char*)m_Buffer;
}

void UITextElement::setFont(Font a_Font)
{
	this->m_Font = a_Font;
}

Font UITextElement::getFont() const
{
	return m_Font;
}

int UITextElement::getCharCount() const
{
	return (int)strlen(m_Buffer);
}

int UITextElement::getMaxCharCount() const
{
	return m_MaxCharCount;
}

void UITextElement::setPlaceholder(const char* a_Text, ColorRGBA a_Color)
{
	m_PlaceholderText = copy_string(a_Text);
	m_PlaceholderFont = { m_Font.key, m_Font.chSize, m_Font.scale, m_Font.spacing, a_Color, m_Font.outline, m_Font.baseline };
}

void UITextElement::centerPos(Vector2 a_ParentPos, FDims a_ParentSize)
{
	int len = getCharCount();

	const float h = m_Font.chSize * m_Font.scale;
	const float w = len * h * m_Font.spacing;

	const float midX = a_ParentPos.x + (a_ParentSize.width / 2.0f);
	const float midY = a_ParentPos.y + (a_ParentSize.height / 2.0f);

	const float x = midX - (w / 2.0f);
	const float y = midY - h/2;

	setSize({ w,h });
	setPosition({ x,y });
}

void UITextElement::leftPos(Vector2 a_ParentPos, FDims a_ParentSize)
{
	int len = getCharCount();

	const float h = m_Font.chSize * m_Font.scale;
	const float w = len * h * m_Font.spacing;

	const float midY = (2 * a_ParentPos.y + a_ParentSize.height) / 2.0f;

	const float x = a_ParentPos.x;
	const float y = midY - m_Font.chSize;

	setSize({ w,h });
	setPosition({ x,y });
}

void UITextElement::draw()
{
	if (getCharCount() == 0 && !m_Focused) {
		m_Mgs->display->drawString(m_Font.key, m_Pos, m_PlaceholderText, m_PlaceholderFont, m_Size);

		return;
	}

	//m_Mgs->display->drawRect(pos, size, ColorRGBA::red(), 2);

	float wt = m_Mgs->time->getWorldTime();

	if (m_Focused && sinf(wt * TEXT_CUR_BLINK_SPD) > 0) {
		const float h = m_Font.chSize * m_Font.scale;
		const float w = getCharCount() * h * m_Font.spacing;
		
		m_Mgs->display->drawFilledRect(
			{ m_Pos.x + w + TEXT_CUR_PAD_LEFT, m_Pos.y - m_Font.baseline },
			{ m_Font.chSize * m_Font.scale * TEXT_CUR_SCALE, m_Size.height },
			m_Font.color,
			ColorRGBA::black(),
			0
		);
	}

	m_Mgs->display->drawString(m_Font.key, m_Pos, m_Buffer, m_Font, m_Size);
}

UIContainer::~UIContainer()
{
	for (auto& el : m_Array) {
		if (el == nullptr) continue;

		delete el;
		el = nullptr;
	}
	m_Array.clear();
}

void UIContainer::addElement(UIElement* a_Element)
{
	m_Array.add(a_Element);
}

void UIContainer::setFocusedElement(int a_Idx)
{
	for (auto& el : m_Array) {
		el->setFocused(false);
	}
	
	if (a_Idx == -1) return;

	m_Array[a_Idx]->setFocused(true);
	m_FocusedElement = a_Idx;
}

int UIContainer::getFocusedElement() const
{
	return m_FocusedElement;
}

int UIContainer::getElementCount() const
{
	return m_Array.count();
}

int UIContainer::getFocusableCount() const
{
	int counter = 0;
	for (auto& el : m_Array) {
		if (el->isFocusable())
			counter++;
	}
	return counter;
}

void UIContainer::update(float a_Dt)
{
	updateElements(a_Dt);
}

void UIContainer::draw()
{
	drawElements();
}

void UIContainer::handleEvents(SDL_Event& a_Event)
{
	if (!m_Active) return;

	detectScroll(a_Event);
	detectHover(a_Event);

	handleEvElements(a_Event);
}

void UIContainer::updateElements(float a_Dt)
{
	if (!m_Active) return;
	for (auto& el : m_Array) {
		if (el != nullptr && el->isActive())
			el->update(a_Dt);
	}
}

void UIContainer::drawElements()
{
	if (!m_Active) return;
	for (auto& el : m_Array) {
		if (el != nullptr && el->isActive())
			el->draw();
	}
}

void UIContainer::handleEvElements(SDL_Event& a_Event)
{
	if (!m_Active) return;
	for (auto& el : m_Array) {
		if (el != nullptr && el->isActive())
			el->handleEvents(a_Event);
	}
}

void UIContainer::detectScroll(SDL_Event& a_Event)
{
	if (!m_Focused && m_Focusable) return;

	if (a_Event.type == SDL_MOUSEWHEEL) {
		onScrollEv(a_Event.wheel.y);
	}

	if (m_Mgs->input->getKey(SDL_SCANCODE_DOWN)) {
		onScrollEv(-1);
	}

	if (m_Mgs->input->getKey(SDL_SCANCODE_UP)) {
		onScrollEv(1);
	}
}

void UIContainer::onScrollEv(int a_Y)
{
	int count = getElementCount();
	if (count <= 1) return;

	int curr = getFocusedElement();
	if (curr < 0) curr = 0;

	int sign = (a_Y > 0) ? -1 : 1;

	// Zmienia focused element na kolejny w danym kierunku
	int next = curr;
	for (int i = 0; i < count; i++) {
		next += sign + count;
		next %= count;

		if (m_Array[next]->isFocusable())
			break;
	}

	setFocusedElement(next);
}

void UIContainer::detectHover(SDL_Event& a_Event)
{
	if (!m_Mgs->display->cursorShown()) return;

	if (a_Event.type == SDL_MOUSEMOTION) {
		Vector2 mousePos = { (float)a_Event.motion.x, (float)a_Event.motion.y };

		for (int i = 0; i < m_Array.count(); i++) {
			Vector2& pos = m_Array[i]->getPosition();
			FDims& size = m_Array[i]->getSize();

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

void UIContainer::onHoverEv(int a_ElIdx) {
	if (m_Array[a_ElIdx]->isFocusable()) {
		setFocusedElement(a_ElIdx);
	}
}

void UIBackgroundContainer::draw()
{
	if (!m_Active) return;

	m_Mgs->display->drawFilledRect(m_Pos, m_Size, m_Background, m_Border, m_BorderSize);

	drawElements();
}

void UIBackgroundContainer::setBackground(ColorRGBA a_Background)
{
	m_Background = a_Background;
}

void UIBackgroundContainer::setBorder(ColorRGBA a_Color, int a_Size)
{
	m_Border = a_Color;
	m_BorderSize = a_Size;
}

ColorRGBA UIBackgroundContainer::getBackground()
{
	return m_Background;
}

ColorRGBA UIBackgroundContainer::getBorderColor()
{
	return m_Border;
}

int UIBackgroundContainer::getBorderSize()
{
	return m_BorderSize;
}

void UISpriteBackgroundContainer::draw()
{
	if (!m_Active) return;

	m_Mgs->display->drawFilledRect(m_Pos, m_Size, m_Background, m_Border, m_BorderSize);

	if (m_SpriteKey != 0)
		m_Mgs->display->drawSprite(m_SpriteKey, m_Pos, m_Size);

	drawElements();
}

void UISpriteBackgroundContainer::setSprite(int a_SpriteKey)
{
	m_SpriteKey = a_SpriteKey;
}

int UISpriteBackgroundContainer::getSprite()
{
	return m_SpriteKey;
}

UIButton::UIButton(Managers* a_Managers, Vector2 a_Pos, FDims a_Size, Font a_Font, void (*a_OnClick)(SDL_Event& a_Event, UIButton* a_Button, Managers* a_Managers))
	: UISpriteBackgroundContainer(a_Managers, a_Pos, a_Size), m_TextElement(a_Managers, a_Pos, a_Size, a_Font), m_OnClick(a_OnClick)
{
	m_Focusable = true;
	m_TextElement.centerPos(m_Pos, a_Size);
}

void UIButton::handleEvents(SDL_Event& a_Event)
{
	Rect rect = { m_Pos.x, m_Pos.y, m_Size.width, m_Size.height };

	if (a_Event.type == SDL_KEYDOWN && a_Event.key.keysym.sym == SDLK_RETURN) {
		if (m_Focused) {
			if (m_OnClick != nullptr)
				m_OnClick(a_Event, this, m_Mgs);
			m_Pressed = true;
			return;
		}
	}
	if (a_Event.type == SDL_KEYUP && a_Event.key.keysym.sym == SDLK_RETURN) {
		m_Pressed = false;
		m_TextElement.centerPos(m_Pos, m_Size);
	}

	if (!m_Mgs->display->cursorShown()) return;

	if (a_Event.type == SDL_MOUSEBUTTONDOWN && a_Event.button.button == SDL_BUTTON_LEFT) {
		float posX = (float)a_Event.button.x;
		float posY = (float)a_Event.button.y;

		if (rect.contains({ posX, posY })) {
			if (m_OnClick != nullptr)
				m_OnClick(a_Event, this, m_Mgs);
			m_Pressed = true;
			return;
		}
	}

	if (a_Event.type == SDL_MOUSEBUTTONUP && a_Event.button.button == SDL_BUTTON_LEFT) {
		m_Pressed = false;
		m_TextElement.centerPos(m_Pos, m_Size);
	}
	
	if (a_Event.type == SDL_MOUSEMOTION) {
		float posX = (float)a_Event.motion.x;
		float posY = (float)a_Event.motion.y;

		if (rect.contains({ posX, posY })) {
			m_Hovered = true;
		}
		else {
			m_Hovered = false;
		}
	}
}

void UIButton::update(float a_Dt)
{
	if (m_AfterPress > 0.0f) {
		m_AfterPress -= a_Dt;

		if (m_AfterPress <= 0.0f) {
			m_Pos.x -= 2;
			m_Pos.y -= 3;
			m_TextElement.centerPos(m_Pos, m_Size);
			m_AfterPress = 0.0f;
		}
	}

	if (m_Pressed) {
		if (m_AfterPress == 0.0f) {
			m_Pos.x += 2;
			m_Pos.y += 3;
			m_TextElement.centerPos(m_Pos, m_Size);
		}
		
		m_Pressed = false;
		m_AfterPress = BTN_PR_DUR;
	}
}

void UIButton::draw()
{
	if (!m_Active) return;

	ColorRGBA dp_bg = m_Background;
	ColorRGBA dp_border = m_Border;

	if (m_Focused) {
		dp_bg = m_Mgs->ui->calcPulse(m_Background);
		dp_border = m_Mgs->ui->calcAlphaPulse(m_Border);
	}

	m_Mgs->display->drawFilledRect(m_Pos, m_Size, dp_bg, dp_border, m_BorderSize);

	if (m_SpriteKey != 0) {
		Sprite* spr = m_Mgs->sprite->get(m_SpriteKey);

		m_Mgs->display->drawSprite(m_SpriteKey, m_Pos, m_Size);

		if (m_Focused) {
			SDL_SetTextureBlendMode(spr->texture, SDL_BLENDMODE_ADD);

			ColorRGBA pulse = m_Mgs->ui->calcPulse(ColorRGBA::black());
			SDL_SetTextureColorMod(spr->texture, pulse.r, pulse.g, pulse.b);

			m_Mgs->display->drawSprite(m_SpriteKey, m_Pos, m_Size);

			SDL_SetTextureBlendMode(spr->texture, SDL_BLENDMODE_BLEND);
		}

		SDL_SetTextureColorMod(spr->texture, 0xFF, 0xFF, 0xFF);
		SDL_SetTextureAlphaMod(spr->texture, 0xFF);
	}
		

	drawElements();

	m_TextElement.draw();
}

void UIButton::setText(const char* a_Text)
{
	m_TextElement.setText(a_Text);
	m_TextElement.centerPos(m_Pos, m_Size);
}

bool UIButton::isPressed() const
{
	return m_Pressed;
}

bool UIButton::isHovered() const
{
	return m_Hovered;
}

UITextInput::UITextInput(Managers* a_Managers, Vector2 a_Pos, FDims a_Size, Font a_Font, int a_MaxChars, Vector2 a_Padding)
	: UISpriteBackgroundContainer(a_Managers, a_Pos, a_Size), 
	m_TextElement(a_Managers, a_Pos, a_Size, a_Font, a_MaxChars) 
{
	m_Focusable = true;
	m_TextElement.setFocusable(true);
	m_TextElement.leftPos(m_Pos + a_Padding, a_Size - (a_Padding * 2));
}

void UITextInput::draw()
{
	if (!m_Active) return;

	m_Mgs->display->drawFilledRect(m_Pos, m_Size, m_Background, m_Border, m_BorderSize);

	if (m_SpriteKey != 0)
		m_Mgs->display->drawSprite(m_SpriteKey, m_Pos, m_Size);

	m_TextElement.draw();
}

void UITextInput::handleEvents(SDL_Event& a_Event)
{
	if (!m_Focused || !m_Active) return;

	char* txt = m_TextElement.getText();
	int charCount = m_TextElement.getCharCount();
	int maxCharCount = m_TextElement.getMaxCharCount();

	if (a_Event.type == SDL_KEYDOWN) {
		if (a_Event.key.keysym.sym == SDLK_BACKSPACE && charCount > 0) {
			txt[--charCount] = '\0';
		}
	}
	else if (a_Event.type == SDL_TEXTINPUT) {

		char first = a_Event.text.text[0];
		if (first < 32) return;

		if (charCount < maxCharCount) {
			strcat_s(txt, (rsize_t)maxCharCount + 1, a_Event.text.text);
		}
	}
}

void UITextInput::setPlaceholder(const char* a_Text, ColorRGBA a_Color)
{
	m_TextElement.setPlaceholder(a_Text, a_Color);
}

void UITextInput::setFocused(bool a_Focused)
{
	if (m_Focusable) {
		m_Focused = a_Focused;
		m_TextElement.setFocused(a_Focused);
	}
}

UIHealthbar::UIHealthbar(Managers* a_Managers, Vector2 a_Pos, FDims a_Size, Vector2 a_Padding)
	: UISpriteBackgroundContainer(a_Managers, a_Pos, a_Size), m_Padding(a_Padding) {}

void UIHealthbar::draw()
{
	if (!m_Active || m_LinkedVal == nullptr) return;

	if (m_SpriteKey != 0)
		m_Mgs->display->drawSprite(m_SpriteKey, m_Pos, m_Size);

	float val = *m_LinkedVal;
	int wf = m_Mgs->time->getWorldFrame();

	Vector2 barPos = m_Pos + m_Padding;
	FDims barSize = m_Size - (m_Padding * 2);

	if (m_GhostKey != 0 && m_GhostVal > val && m_GhostVal > 0) {
		float progress = (m_Max != 0) ? m_GhostVal / m_Max : 0.0f;

		Sprite* spr = m_Mgs->sprite->get(m_GhostKey);
		m_Mgs->display->drawClippedSprite(
			m_GhostKey, 
			barPos,
			{ barSize.width * progress, barSize.height },
			{ 
				0, 0, 
				(int)(spr->width * progress), 
				spr->height 
			}
		);
	}

	if (m_FillKey != 0 && val > 0) {
		float progress = (m_Max != 0) ? val / m_Max : 0.0f;

		Sprite* spr = m_Mgs->sprite->get(m_FillKey);
		m_Mgs->display->drawClippedSprite(
			m_FillKey,
			barPos,
			{ barSize.width * progress, barSize.height},
			{
				0, 0,
				(int)(spr->width * progress),
				spr->height
			}
		);
	}

	drawElements();
}

void UIHealthbar::update(float a_Dt)
{
	if (!m_Active || m_LinkedVal == nullptr) return;

	float val = *m_LinkedVal;

	if (m_GhostVal > val) {
		float dVal = m_GhostVal - val;

		float speed = dVal * HB_GHOST_SPD + HB_GHOST_BASE;

		m_GhostVal -= speed * a_Dt;
	}

	if (m_GhostVal < val)
		m_GhostVal = val;
}

float UIHealthbar::getMax() const
{
	return m_Max;
}

void UIHealthbar::setMax(float a_Max)
{
	m_Max = a_Max;
}

float UIHealthbar::getVal() const
{
	return *m_LinkedVal;
}

void UIHealthbar::linkVals(float* a_Val)
{
	m_LinkedVal = a_Val;
	m_GhostVal = *a_Val;
}

void UIHealthbar::setFill(int a_Key)
{
	m_FillKey = a_Key;
}

int UIHealthbar::getFill()
{
	return m_FillKey;
}

void UIHealthbar::setGhost(int a_Key)
{
	m_GhostKey = a_Key;
}

int UIHealthbar::getGhost()
{
	return m_GhostKey;
}
