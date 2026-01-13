#pragma once

#include "../../core/core.h"
#include "../config.h"
#include "actor.h"

#include "player.h"

class Enemy : public Actor {
protected:
	Actor* m_Target{};
	float m_HP{ DEF_HP };
	bool m_AttackXRequested{ false };
	bool m_AttackYRequested{ false };
	bool m_JumpRequested{ false };
	bool m_Attacking{ false };
	Font m_DebugActionFont;

	virtual int getAnimFromAct(int a_ActKey) const override = 0;
	virtual void computeInput() override = 0;
public:
	Enemy(Managers* a_Managers, Actor* a_Target = nullptr, Transform a_Transform = Transform::zero());
	virtual ~Enemy() override;
	virtual void start() override = 0;
	virtual void update(float a_Dt) override;

	virtual Uint8 getType() const override;
};