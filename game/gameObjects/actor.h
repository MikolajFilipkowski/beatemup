#pragma once

#include "../../core/core.h"
#include "../config.h"

class Actor : public GameObject {
protected:
	int m_Id;

	int m_CurrentActKey{ 0 };
	int m_CurrentAnimKey{ 0 };
	float m_ActTimer{ 0 };
	int m_CurrentFrame{ 0 };

	bool m_Grounded{ true };
	bool m_Attacking{ false };

	float m_HP{ DEF_HP };
	float m_InvTimer{ 0.0f };
	float m_LastDmgTaken{ 0.0f };

	float m_DeathTimer{ 0.0f };
	float m_IsDying{ false };

	Vector3 m_InputVel{};
	FacingDir m_FacingDir{ FacingDir::RIGHT };

	virtual int getAnimFromAct(int a_ActKey) const;
	virtual void computeInput();
	int getActiveAnim();
	void loadAnims();
	void unloadAnims();
public:
	Actor(Managers* a_Managers, Transform a_Transform = Transform::zero());
	virtual ~Actor();

	int getCurrentAnimKey() const;
	void setAnim(int a_AnimKey);

	void startAction(int a_ActKey);
	virtual void optActionAdjust(int a_ActKey) {}
	ActionData* getCurrAction();
	ActionFrame& getCurrFrame();

	virtual void fixedUpdate(float a_FixedDt) override;
	virtual void applyPhysics(float a_FixedDt, ActionData* a_Data, ActionFrame& a_CurrFrame);

	virtual Cuboid getCollBox() override;

	virtual void actionFinish() {}
	virtual void die() {}
	bool isDying() const;

	virtual void postFixedUpdate(float a_FixedDt) override;

	virtual void draw() override;
	void drawCollBoxes();
	FacingDir getFacingDir() const;

	virtual Uint8 getStateMask() const;

	bool checkHit(Actor* a_Victim);
	bool canBeHit(Actor* a_Attacker) const;
	void registerHit(Actor* a_Attacker);
	virtual void takeDamage(float a_Dmg);

	float& getHP();
};