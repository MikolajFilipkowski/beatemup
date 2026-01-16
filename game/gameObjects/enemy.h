#pragma once

#include "../../core/core.h"
#include "../config.h"
#include "actor.h"

#include "player.h"

class Enemy : public Actor {
protected:
	
	Actor* m_Target{};
	float m_HP{ DEF_HP };
	float m_AttackTimer{0.0f};
	bool m_Attacking{ false };
	Font m_DebugActionFont;
	Map<int, int> m_AttackChances{};

	virtual int getAnimFromAct(int a_ActKey) const override = 0;
	virtual void computeInput() override = 0;
	bool chooseAttack();
public:
	Enemy(Managers* a_Managers, GameState* a_GameState, Actor* a_Target = nullptr, Transform a_Transform = Transform::zero());
	virtual ~Enemy() override;
	virtual void start() override = 0;
	virtual void update(float a_Dt) override;

	virtual void die() override;

	virtual Uint8 getType() const override;

	int totalChance() const;
	
	bool inAttackRange(int a_ActKey) const;
	virtual float getAttackCooldown() const;
};