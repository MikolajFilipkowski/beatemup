#pragma once

#include "enemy.h"

class Autumn : public Enemy {
private:
	inline static int s_Instances = 0;
	virtual int getAnimFromAct(int a_ActKey) const override;
	virtual void computeInput() override;
public:
	Autumn(Managers* a_Managers, Actor* a_Target = nullptr, Transform a_Transform = Transform::zero());
	virtual ~Autumn();
	void start() override;
	void update(float a_Dt) override;
};