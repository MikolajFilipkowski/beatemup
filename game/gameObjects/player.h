#pragma once

#include "../../core/core.h"
#include "../config.h"
#include "../input_buffer.h"

enum class FacingDir {
	LEFT,
	RIGHT
};

class Player : public Actor {
protected:
	InputBuffer m_IBuffer;
	float m_HP{ DEF_HP };
	bool m_AttackXRequested{ false };
	bool m_AttackYRequested{ false };
	bool m_JumpRequested{ false };
	bool m_Attacking{ false };
	float m_AttackTimer{ 0 };
	FacingDir m_FacingDir{ FacingDir::RIGHT };
	Font m_DebugActionFont;

	int getAnimKeyFromAct(int a_ActKey) override;
	virtual void computeInput() override;
	int getActiveAnim();
public:
	Player(Managers* a_Managers, Transform a_Transform = Transform::zero());
	virtual ~Player() override;
	virtual void start() override;
	virtual void update(float a_Dt) override;
	
	virtual void draw() override;
	void drawActionName();

	InputBuffer& getIBuffer();
	FacingDir getFacingDir() const;
};