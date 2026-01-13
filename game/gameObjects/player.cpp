#include "player.h"
#include <cstdio>
#include "enemy.h"

#define PLY_ASSETS "game/assets/player/ply_"

int Player::getAnimFromAct(int a_ActKey) const
{
	switch (a_ActKey) {
	case Actions::IDLE: 
		return RES::PLY_IDLE;
	case Actions::WALK: 
		return RES::PLY_WALK;
	case Actions::JUMP: 
		return RES::PLY_JUMP;
	case Actions::LIGHT_ATTACK: 
		return RES::PLY_LIGHT_ATT;
	case Actions::HEAVY_ATTACK: 
		return RES::PLY_HEAVY_ATT;
	case Actions::WHEEL_PUNCH:
		return RES::PLY_WHEEL_PUNCH;
	default: 
		return RES::NONE;
	}
}

void Player::computeInput()
{
	m_InputVel = Vector3::zero();

	ActionData* currAct = m_Mgs->object->getAction(m_CurrentActKey);
	bool canInterrupt = (currAct == nullptr || currAct->interruptible);
	bool isMoving = false;

	// Zmiana stanu inputEntries z IN_USE na USED po animacji + delay
	if (m_BufferDecay > 0) {
		m_BufferDecay--;

		if (m_BufferDecay <= 0)
			m_IBuffer.flagUsed();
	}

	if (currAct->canMove) {
		float currSpeed = (m_Grounded) ? PLY_SPEED : PLY_SPEED * IN_AIR_MUL;

		if (m_Mgs->input->getInput(InputBind::RIGHT))
			m_InputVel.x += currSpeed;
		if (m_Mgs->input->getInput(InputBind::LEFT))
			m_InputVel.x -= currSpeed;

		if (m_Mgs->input->getInput(InputBind::UP))
			m_InputVel.z -= currSpeed * Z_AXIS_MUL;
		if (m_Mgs->input->getInput(InputBind::DOWN))
			m_InputVel.z += currSpeed * Z_AXIS_MUL;

		
		if (m_InputVel.x != 0) {
			m_Transform.flip = (m_InputVel.x > 0) ? NO_FLIP : H_FLIP;
			isMoving = true;
		}

		if (m_InputVel.z != 0)
			isMoving = true;
	}
	

	if (!canInterrupt) return;
	int buffActKey = m_IBuffer.getNextAct();

	if (buffActKey == m_CurrentActKey) return;

	if (canInterrupt && buffActKey != Actions::NONE) {
		ActionData* buffAct = m_Mgs->object->getAction(buffActKey);
		if (buffAct->priority > currAct->priority)
			startAction(buffActKey);

		return;
	}

	int walkPrio = m_Mgs->object->getAction(Actions::WALK)->priority;
	if (currAct->priority > walkPrio) return;

	if (isMoving) {
		startAction(Actions::WALK);
	}
	else {
		startAction(Actions::IDLE);
	}
}

Player::Player(Managers* a_Managers, Transform a_Transform)
	: Actor(a_Managers, a_Transform), m_IBuffer(a_Managers, this) 
{
	m_DebugActionFont = Font(RES::CH_16, 16, 3.0, 1.0, Colors::white, {Colors::black, 2}, 0);
}

Player::~Player()
{
	unloadAnims();
}

void Player::start()
{
	m_Mgs->sprite->load(PLY_ASSETS "idle.bmp", getAnimFromAct(Actions::IDLE));
	m_Mgs->sprite->load(PLY_ASSETS "walk.bmp", getAnimFromAct(Actions::WALK));
	m_Mgs->sprite->load(PLY_ASSETS "jump.bmp", getAnimFromAct(Actions::JUMP));
	m_Mgs->sprite->load(PLY_ASSETS "attack1.bmp", getAnimFromAct(Actions::LIGHT_ATTACK));
	m_Mgs->sprite->load(PLY_ASSETS "attack2.bmp", getAnimFromAct(Actions::HEAVY_ATTACK));
	m_Mgs->sprite->load(PLY_ASSETS "attack3.bmp", getAnimFromAct(Actions::WHEEL_PUNCH));

	loadAnims();

	startAction(Actions::IDLE);
}

void Player::update(float a_Dt)
{
	m_IBuffer.update();

	m_Transform.pos = getIPos();
}

void Player::optActionAdjust(int a_ActKey)
{
	m_BufferDecay = 0;

	if (!m_Mgs->object->getAction(a_ActKey)->interruptible)
		m_IBuffer.flagUsed();
}

void Player::actionFinish()
{
	m_BufferDecay = BUFFER_DECAY;
}

Uint8 Player::getType() const
{
	return ObjectType::PLAYER;
}

void Player::drawActionName()
{
	const char* actName = m_Mgs->object->getAction(m_CurrentActKey)->name;
	float logW = (float)m_Mgs->display->getLogWidth();

	float txtW = strlen(actName) * m_DebugActionFont.chSize * m_DebugActionFont.scale * m_DebugActionFont.spacing;

	float posX = logW / 2.0f - txtW / 2.0f;
	float posY = 50.0f;

	m_Mgs->display->drawString(RES::CH_16, { posX, posY }, actName, m_DebugActionFont);
}

void Player::takeDamage(float a_Dmg)
{
	m_HP -= a_Dmg;

	if (m_HP < 0) {
		printf("Koniec gry!");
		m_Mgs->engine->stop();
	}
}

InputBuffer& Player::getIBuffer() 
{
	return m_IBuffer;
}

