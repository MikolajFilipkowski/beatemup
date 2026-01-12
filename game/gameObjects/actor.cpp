#include "actor.h"

int Actor::getAnimKeyFromAct(int a_ActKey)
{
	return 0;
}

void Actor::computeInput()
{
	m_InputVel = { 0,0,0 };
}

int Actor::getActiveAnim()
{
	return getAnimKeyFromAct(m_CurrentActKey);
}

Actor::Actor(Managers* a_Managers, Transform a_Transform)
	: GameObject(a_Managers, a_Transform) {
}

Actor::~Actor() {

}

int Actor::getCurrentAnimKey() const
{
	return m_CurrentAnimKey;
}

void Actor::setAnim(int a_AnimKey)
{
	if (a_AnimKey == 0 || m_CurrentAnimKey == a_AnimKey) return;

	m_CurrentAnimKey = a_AnimKey;
}

void Actor::startAction(int a_ActKey)
{
	if (m_CurrentActKey == a_ActKey) return;

	ActionData* data = m_Mgs->object->getAction(a_ActKey);
	if (data == nullptr) return;

	m_CurrentActKey = a_ActKey;
	m_ActTimer = 0.0f;
	m_CurrentFrame = 0;

	int animKey = getAnimKeyFromAct(a_ActKey);
	setAnim(animKey);
}

ActionData* Actor::getCurrAction()
{
	return m_Mgs->object->getAction(m_CurrentActKey);
}

ActionFrame& Actor::getCurrFrame()
{
	return getCurrAction()->getFrames()->get(m_CurrentFrame);
}

void Actor::fixedUpdate(float a_FixedDt)
{
	// Do interpolacji klatek ruchu
	m_Rb.prevPos = m_Rb.currPos;

	m_FacingDir = (m_Transform.flip == NO_FLIP) ? FacingDir::RIGHT : FacingDir::LEFT;

	if (m_CurrentActKey == 0) return;
	ActionData* data = m_Mgs->object->getAction(m_CurrentActKey);

	m_ActTimer += a_FixedDt;

	float totalDur = data->getTotalDuration();
	if (m_ActTimer >= totalDur) {
		// Jesli interruptible zapetlenie, jesli nie - powrot do akcji podstawowej 
		if (data->interruptible) {
			m_ActTimer = fmodf(m_ActTimer, totalDur);
		}
		else {
			m_Attacking = false;
			startAction(1);
			return;
		}
	}

	auto frames = data->getFrames();

	// Kalkulacja nr klatki
	int frameIdx = 0;
	float accum = 0.0f;
	for (int i = 0; i < frames->count(); i++) {
		accum += frames->get(i).duration;

		if (accum >= m_ActTimer) {
			frameIdx = i;
			break;
		}
	}

	m_CurrentFrame = frameIdx;
	auto& currFrame = frames->get(frameIdx);

	applyPhysics(a_FixedDt, data, currFrame);
}

void Actor::applyPhysics(float a_FixedDt, ActionData* a_Data, ActionFrame& a_CurrFrame)
{
	Vector3& pos = m_Rb.currPos;

	float lookDir = (m_Transform.flip == H_FLIP) ? -1.0f : 1.0f;


	computeInput();
	/*
	if (a_Data->interruptible || !m_Grounded)
		computeInput();
	else
		m_InputVel = Vector3::zero();*/

	// Zmiana predkosci
	m_Rb.vel.x = (a_CurrFrame.vel.x * lookDir) + m_InputVel.x;
	m_Rb.vel.z = a_CurrFrame.vel.z + m_InputVel.z;

	if (a_CurrFrame.vel.y != 0)
		m_Rb.vel.y = a_CurrFrame.vel.y;

	float gravity = m_Mgs->object->getGravity();

	if (!m_Grounded) {
		m_Rb.vel.y += gravity * a_FixedDt;
	}

	// Zmiana pozycji na bazie predkosci
	pos.x += m_Rb.vel.x * a_FixedDt;
	pos.y += m_Rb.vel.y * a_FixedDt;
	pos.z += m_Rb.vel.z * a_FixedDt;

	if (pos.y >= 0) {
		pos.y = 0;
		m_Rb.vel.y = 0;

		if (!m_Grounded) {
			m_Grounded = true;
			startAction(1);
		}
	}
	else {
		m_Grounded = false;
	}
}

void Actor::postFixedUpdate(float a_FixedDt)
{
	if (!m_Attacking) return;

	auto& objArr = m_Mgs->object->getAllObjects();

	for (auto& target : objArr) {
		if (target == nullptr || target == this ||
			target->getRemovalFlag() || !(target->getType() & ObjectType::ACTOR)
		) {
			continue;
		}
			
		Actor* victim = (Actor*)target;
		Uint8 vicMask = victim->getType() & (ObjectType::ENEMY | ObjectType::PLAYER);
		Uint8 attMask = getType() & (ObjectType::ENEMY | ObjectType::PLAYER);

		// By nie bylo friendly-fire
		if (vicMask == attMask) continue;

		if (victim->canBeHit(this) && victim->checkHit(this)) {
			victim->registerHit(this);
		}
	}
}

void Actor::drawCollBoxes()
{
	ActionData* currAction = m_Mgs->object->getAction(m_CurrentActKey);
	ActionFrame& frame = currAction->getFrames()->get(m_CurrentFrame);

	Vector2 screenPos = m_Mgs->display->worldToScreen(getIPos());

	bool facingRight = getFacingDir() == FacingDir::RIGHT;

	if (frame.hitbox.h > 0 && frame.hitbox.w > 0) {
		float posX = screenPos.x;
		posX += (facingRight) ? frame.hitbox.x : -frame.hitbox.x - frame.hitbox.w;

		Vector2 hitPos = {
			posX,
			screenPos.y - frame.hitbox.y - frame.hitbox.h
		};

		FDims hitDims = {
			frame.hitbox.w,
			frame.hitbox.h
		};

		m_Mgs->display->drawRect(hitPos, hitDims, ColorRGBA::red(), 2);
	}

	if (frame.hurtbox.h > 0 && frame.hurtbox.w > 0) {
		float posX = screenPos.x;
		posX += (facingRight) ? frame.hurtbox.x : -frame.hurtbox.x - frame.hurtbox.w;

		Vector2 hurtPos = {
			posX,
			screenPos.y - frame.hurtbox.y - frame.hurtbox.h
		};

		FDims hurtDims = {
			frame.hurtbox.w,
			frame.hurtbox.h
		};

		m_Mgs->display->drawRect(hurtPos, hurtDims, ColorRGBA::green(), 2);
	}
}

FacingDir Actor::getFacingDir() const
{
	return m_FacingDir;
}

bool Actor::checkHit(Actor* a_Victim)
{
	if (a_Victim == this) 
		return false;

	Vector3& attPos = getRb().currPos;
	Vector3& vicPos = a_Victim->getRb().currPos;
	float zDist = fabs(attPos.z - vicPos.z);

	if (zDist > MAX_DZ_HIT)
		return false;

	ActionFrame& attFr = getCurrFrame();
	ActionFrame& vicFr = a_Victim->getCurrFrame();

	if (attFr.hitbox.w == 0 || attFr.hitbox.h == 0 ||
		vicFr.hurtbox.w == 0 || vicFr.hurtbox.h == 0
		) {
		return false;
	}

	bool attRight = getFacingDir() == FacingDir::RIGHT;
	bool vicRight = a_Victim->getFacingDir() == FacingDir::RIGHT;

	float hitX = attPos.x;
	hitX += (attRight) ? attFr.hitbox.x : -attFr.hitbox.x - attFr.hitbox.w;
	Rect attHitbox = {
		hitX,
		attPos.y - attFr.hitbox.y - attFr.hitbox.h,
		attFr.hitbox.w,
		attFr.hitbox.h
	};

	float hurtX = vicPos.x;
	hurtX += (vicRight) ? vicFr.hurtbox.x : -vicFr.hurtbox.x - vicFr.hurtbox.w;

	Rect vicHurtbox = {
		hurtX,
		vicPos.y - vicFr.hurtbox.y - vicFr.hurtbox.h,
		vicFr.hurtbox.w,
		vicFr.hurtbox.h
	};

	return attHitbox.intersects(vicHurtbox);
}

bool Actor::canBeHit(Actor* a_Attacker)
{
	float dmg = a_Attacker->getCurrFrame().damage;

	if (m_InvTimer <= 0.0f || dmg > m_LastDmgTaken) return true;

	return false;
}

void Actor::registerHit(Actor* a_Attacker)
{
	float dmg = a_Attacker->getCurrFrame().damage;
	float actualDmg = dmg;

	if (m_InvTimer > 0.0f && dmg > m_LastDmgTaken)
		actualDmg -= m_LastDmgTaken;

	takeDamage(actualDmg);

	m_LastDmgTaken = actualDmg;
	m_InvTimer = AFTER_HIT_INV;
}

void Actor::takeDamage(float a_Dmg)
{
	m_HP -= a_Dmg;

	if (m_HP <= 0) {
		m_Mgs->object->remove(this);
	}
}

float& Actor::getHP()
{
	return m_HP;
}
