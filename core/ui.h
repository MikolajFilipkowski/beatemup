#pragma once

#include "core_types.h"

inline constexpr float HB_GHOST_SPD = 5.0f;
inline constexpr float HB_GHOST_BASE = 15.0f;

class UIElement {
protected:
	Managers* m_Mgs;
	Vector2 m_Pos;
	FDims m_Size;
	bool m_Active{ true };
	bool m_Focused{ false };
	bool m_Focusable{ false };
	int m_Id{ -1 };
	bool m_ShouldFollow{ false };
public:
	UIElement(Managers* a_Managers, Vector2 a_Pos, FDims a_Size);
	UIElement(const UIElement&) = delete;
	UIElement& operator=(const UIElement&) = delete;
	virtual ~UIElement() = default;
	virtual void update(float a_Dt) {}
	virtual void handleEvents(SDL_Event& a_Event) {}
	virtual void draw() = 0;

	int getId() const { return m_Id; }
	void setId(int a_Id) { m_Id = a_Id; }

	Vector2& getPosition() { return m_Pos; }
	FDims& getSize() { return m_Size; }
	void setPosition(Vector2 a_Pos);
	void setSize(FDims a_Size) { this->m_Size = a_Size; }
	bool isActive() const { return m_Active; }
	void setActive(bool a_Active) { m_Active = a_Active; }
	bool isFocused() const { return m_Focused; }
	virtual void setFocused(bool a_Focused);

	bool isFollowing() const { return m_ShouldFollow; }
	void setFollow(bool a_Follow) { m_ShouldFollow = a_Follow; }

	bool isFocusable() const { return m_Focusable; }
	void setFocusable(bool a_Focusable) { m_Focusable = a_Focusable; }

	bool isMouseOver(Vector2 a_MousePos);

	virtual void onChangePos(Vector2 a_OldPos) {}
	virtual void onParentChangedPos(Vector2 a_OldPos, Vector2 a_NewPos) {}
};

class UITextElement : public UIElement {
protected:
	char m_Buffer[MAX_TEXTSIZE]{};
	int m_MaxCharCount{};
	Font m_Font{};
	char* m_PlaceholderText{};
	Font m_PlaceholderFont;
public:
	UITextElement(Managers* a_Managers, Vector2 a_Pos, FDims a_Size, Font a_Font, int a_MaxChars = 255);
	virtual ~UITextElement();
	void setText(const char* a_Text);
	char* getText() const;
	void setFont(Font a_Font);
	Font getFont() const;

	int getCharCount() const;
	int getMaxCharCount() const;

	void setPlaceholder(const char* a_Text, ColorRGBA a_Color);

	void centerPos(Vector2 a_ParentPos, FDims a_ParentSize);
	void leftPos(Vector2 a_ParentPos, FDims a_ParentSize);

	virtual void draw() override;
	virtual void onParentChangedPos(Vector2 a_OldPos, Vector2 a_NewPos) override;
};

class UIContainer : public UIElement {
protected:
	ArrayList<UIElement*> m_Array{};
	int m_FocusedElement{ -1 };
public:
	UIContainer(Managers* a_Managers, Vector2 a_Pos, FDims a_Size) 
		: UIElement(a_Managers, a_Pos, a_Size) {}
	virtual ~UIContainer();
	virtual void addElement(UIElement* a_Element);
	void setFocusedElement(int a_Idx);
	int getFocusedElement() const;

	int getElementCount() const;
	int getFocusableCount() const;

	virtual void update(float a_Dt) override;
	virtual void draw() override;
	virtual void handleEvents(SDL_Event& a_Event) override;

	void updateElements(float a_Dt);
	void drawElements();
	void handleEvElements(SDL_Event& a_Event);

	void detectScroll(SDL_Event& a_Event);
	virtual void onScrollEv(int a_Y);

	void detectHover(SDL_Event& a_Event);
	virtual void onHoverEv(int a_ElIdx);

	virtual void onChangePos(Vector2 a_OldPos) override;
};

class UIBackgroundContainer : public UIContainer {
protected:
	ColorRGBA m_Background{ ColorRGBA::transparent() };
	ColorRGBA m_Border{ ColorRGBA::transparent() };
	int m_BorderSize{ 0 };
public:
	UIBackgroundContainer(Managers* a_Managers, Vector2 a_Pos, FDims a_Size) 
		: UIContainer(a_Managers, a_Pos, a_Size) {}
	virtual ~UIBackgroundContainer() = default;

	virtual void draw() override;
	void setBackground(ColorRGBA a_Background);
	void setBorder(ColorRGBA a_Color, int a_Size);

	ColorRGBA getBackground();
	ColorRGBA getBorderColor();
	int getBorderSize();
};

class UISpriteContainer : public UIBackgroundContainer {
protected:
	int m_SpriteKey{ 0 };
public:
	UISpriteContainer(Managers* a_Managers, Vector2 a_Pos, FDims a_Size)
		: UIBackgroundContainer(a_Managers, a_Pos, a_Size) {}
	virtual ~UISpriteContainer() = default;

	virtual void draw() override;
	void setSprite(int a_Key);
	int getSprite();
};

class UIButton : public UISpriteContainer {
protected:
	UITextElement m_TextElement;
	bool m_Pressed{ false };
	bool m_Hovered{ false };
	float m_AfterPress{ 0 };
	void (*m_OnClick)(SDL_Event& a_Event, UIButton* a_Button, Managers* a_Managers);
	Vector2 m_Padding{};
public:
	UIButton(Managers* a_Managers, Vector2 a_Pos, FDims a_Size, Font a_Font,
		void (*m_OnClick)(SDL_Event& a_Event, UIButton* a_Button, Managers* a_Managers) = nullptr
		);
	virtual void handleEvents(SDL_Event& a_Event) override;
	virtual void update(float a_Dt) override;
	virtual void draw() override;
	virtual void setText(const char* a_Text);
	
	bool isPressed() const;
	bool isHovered() const;
};

class UITextInput : public UISpriteContainer {
protected:
	UITextElement m_TextElement;
public:
	UITextInput(Managers* a_Managers, Vector2 a_Pos, FDims a_Size, Font a_Font, int a_MaxChars = 255, Vector2 a_Padding = {0,0});
	virtual ~UITextInput() = default;
	virtual void draw() override;
	virtual void handleEvents(SDL_Event& a_Event) override;
	void setPlaceholder(const char* a_Text, ColorRGBA a_Color);
	virtual void setFocused(bool a_Focused) override;
	const char* getContent() const;
};

class UIHealthbar : public UISpriteContainer {
protected:
	Vector2 m_Padding;
	float m_Max{ 0 };
	float* m_LinkedVal{};
	int m_FillKey{ 0 };
	int m_GhostKey{ 0 };
	float m_GhostVal{ 0 };
public:
	UIHealthbar(Managers* a_Managers, Vector2 a_Pos, FDims a_Size, Vector2 a_Padding = { 0,0 });
	virtual void draw() override;
	virtual void update(float a_Dt) override;

	float getMax() const;
	void setMax(float a_Max);

	float getVal() const;
	void linkVals(float* a_Val);

	void setFill(int a_Key);
	int getFill() const;
	void setGhost(int a_Key);
	int getGhost();
};

static constexpr float DEF_SCROLL_FACTOR = 20.0f;
class UIScrollableContainer : public UISpriteContainer {
	float m_ScrollFactor{ DEF_SCROLL_FACTOR };
	float m_ScrollPos{ 0 };
	Rect m_Viewport;
	Vector2 m_Padding;

	void calculateViewport();
public:
	UIScrollableContainer(Managers* a_Managers, Vector2 a_Pos, FDims a_Size, Vector2 a_Padding = { 0,0 });
	virtual void draw() override;
	virtual void onScrollEv(int a_Y) override;
	virtual void addElement(UIElement* a_Element) override;

	void setScrollFactor(float a_ScrollFactor);
	float getScrollFactor() const;

	const Rect& getViewport() const;
};