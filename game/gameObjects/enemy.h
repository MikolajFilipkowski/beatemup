#pragma once

#include "../../core/core.h"
#include "../config.h"
#include "actor.h"

#include "player.h"

class Enemy : public Actor {
protected:
	Player* m_Player{};
	float m_HP{ DEF_HP };
	bool m_AttackXRequested{ false };
	bool m_AttackYRequested{ false };
	bool m_JumpRequested{ false };
	bool m_Attacking{ false };
	float m_AttackTimer{ 0 };
	Font m_DebugActionFont;
public:
	Enemy(Managers* a_Managers, Player* a_Player = nullptr, Transform a_Transform = Transform::zero());
	virtual ~Enemy() override;
	virtual void start() override;
	virtual void update(float a_Dt) override;

	virtual Uint8 getType() const override;

	virtual void draw() override;
};