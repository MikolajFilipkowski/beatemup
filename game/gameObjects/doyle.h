#pragma once

#include "enemy.h"

class Doyle : public Enemy {
private:
	inline static int s_Instances = 0;
	virtual int getAnimFromAct(int a_ActKey) const override;
	virtual void computeInput() override;
public:
	Doyle(Managers* a_Managers, Actor* a_Target = nullptr, Transform a_Transform = Transform::zero());
	virtual ~Doyle();
	void start() override;
	void update(float a_Dt) override;
};