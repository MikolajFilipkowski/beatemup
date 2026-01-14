#pragma once

#include "../../core/core.h"
#include "../config.h"
#include "actor.h"
#include "../input_buffer.h"

class Player : public Actor {
protected:
	InputBuffer m_IBuffer;
	int m_BufferDecay{ 0 };
	Font m_DebugActionFont;

	int getAnimFromAct(int a_ActKey) const override;
	virtual void computeInput() override;
	
public:
	Player(Managers* a_Managers, Transform a_Transform = Transform::zero());
	virtual ~Player() override;
	virtual void start() override;
	virtual void update(float a_Dt) override;

	virtual void optActionAdjust(int a_ActKey) override;
	virtual void actionFinish() override;
	virtual void die() override;

	virtual Uint8 getType() const override;
	
	void drawActionName();

	InputBuffer& getIBuffer();
};