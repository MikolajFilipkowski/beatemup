#pragma once

#include "enemy.h"

class Autumn : public Enemy {
private:
	static constexpr int LIGHT_ATT_CHANCE = 50;
	static constexpr int HEAVY_ATT_CHANCE = 70;
	static constexpr int WHEEL_ATT_CHANCE = 100;
	static constexpr float ATT_COOLDOWN = 0.2f;
	static constexpr float DASH_COOLDOWN = 5.0f;
	static constexpr float CHARGE_COOLDOWN = 2.0f;

	float m_DashTimer{ 0.0f };
	float m_ChargeTimer{ 0.0f };
	inline static int s_Instances = 0;
	virtual int getAnimFromAct(int a_ActKey) const override;
	bool calcMovement(float& a_DiffX, float& a_DiffZ, bool& a_IsMoving);
	virtual void computeInput() override;
	
public:
	Autumn(Managers* a_Managers, GameState* a_GameState, Actor* a_Target, Transform a_Transform, int& a_EnemyCount);
	virtual ~Autumn();
	void start() override;
	void update(float a_Dt) override;

	float getAttackCooldown() const override;
};