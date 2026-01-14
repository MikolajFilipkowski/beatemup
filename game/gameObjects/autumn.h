#pragma once

#include "enemy.h"

class Autumn : public Enemy {
private:
	static constexpr int LIGHT_ATT_CHANCE = 50;
	static constexpr int HEAVY_ATT_CHANCE = 70;
	static constexpr int WHEEL_ATT_CHANCE = 100;
	static constexpr float ATT_COOLDOWN = 1.5f;

	inline static int s_Instances = 0;
	virtual int getAnimFromAct(int a_ActKey) const override;
	virtual void computeInput() override;
	
public:
	Autumn(Managers* a_Managers, Actor* a_Target = nullptr, Transform a_Transform = Transform::zero());
	virtual ~Autumn();
	void start() override;
	void update(float a_Dt) override;

	float getAttackCooldown() const override;
};