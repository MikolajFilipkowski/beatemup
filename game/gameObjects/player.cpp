#include "player.h"
#include <cstdio>
#include "enemy.h"

#define PLY_ASSETS "game/assets/player/"

int Player::getAnimKeyFromAct(int a_ActKey)
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

	float currSpeed = (m_Grounded) ? PLY_SPEED : PLY_SPEED * IN_AIR_MUL;

	if (m_Mgs->input->getInput(InputBind::RIGHT))
		m_InputVel.x += currSpeed;
	if (m_Mgs->input->getInput(InputBind::LEFT))
		m_InputVel.x -= currSpeed;

	if (m_Mgs->input->getInput(InputBind::UP))
		m_InputVel.z -= currSpeed * Z_AXIS_MUL;
	if (m_Mgs->input->getInput(InputBind::DOWN))
		m_InputVel.z += currSpeed * Z_AXIS_MUL;


	bool isMoving = false;
	if (m_InputVel.x != 0) {
		m_Transform.flip = (m_InputVel.x > 0) ? NO_FLIP : H_FLIP;
		isMoving = true;
	}

	if (m_InputVel.z != 0)
		isMoving = true;

	if (!m_Grounded) return;

	ActionData* currAct = m_Mgs->object->getAction(m_CurrentActKey);
	bool canInterrupt = (currAct == nullptr || currAct->interruptible);

	int wf = m_Mgs->time->getWorldFrame();

	if (m_IBuffer.matches(Actions::WHEEL_PUNCH, wf)) {
		m_Attacking = true;
		startAction(Actions::WHEEL_PUNCH);
		m_IBuffer.clear();
		return;
	}

	if (canInterrupt && m_JumpRequested) {
		m_JumpRequested = false;
		m_Grounded = false;

		m_HP -= 15.0f;

		m_Rb.vel.y = JUMP_FORCE;
		startAction(Actions::JUMP);
		return;
	}

	if (canInterrupt && m_AttackXRequested) {
		m_AttackXRequested = false;
		m_Attacking = true;
		startAction(Actions::LIGHT_ATTACK);
		return;
	}

	if (canInterrupt && m_AttackYRequested) {
		m_AttackYRequested = false;
		m_Attacking = true;
		startAction(Actions::HEAVY_ATTACK);
		return;
	}

	if (canInterrupt && isMoving) {
		startAction(Actions::WALK);
		return;
	}

	if (canInterrupt && !isMoving) {
		startAction(Actions::IDLE);
		return;
	}
}

Player::Player(Managers* a_Managers, Transform a_Transform)
	: Actor(a_Managers, a_Transform), m_IBuffer(a_Managers, this) 
{
	m_DebugActionFont = Font(RES::CH_16, 16, 3.0, 1.0, Colors::white, {Colors::black, 2}, 0);
}

Player::~Player()
{
}

void Player::start()
{
	m_Mgs->sprite->load(PLY_ASSETS "ply_idle.bmp", RES::PLY_IDLE);
	m_Mgs->anim->createFromSheet(RES::PLY_IDLE, RES::PLY_IDLE, 7);

	m_Mgs->sprite->load(PLY_ASSETS "ply_walk.bmp", RES::PLY_WALK);
	m_Mgs->anim->createFromSheet(RES::PLY_WALK, RES::PLY_WALK, 13);

	m_Mgs->sprite->load(PLY_ASSETS "ply_jump.bmp", RES::PLY_JUMP);
	m_Mgs->anim->createFromSheet(RES::PLY_JUMP, RES::PLY_JUMP, 13);

	m_Mgs->sprite->load(PLY_ASSETS "ply_attack1.bmp", RES::PLY_LIGHT_ATT);
	m_Mgs->anim->createFromSheet(RES::PLY_LIGHT_ATT, RES::PLY_LIGHT_ATT, 12);

	m_Mgs->sprite->load(PLY_ASSETS "ply_attack2.bmp", RES::PLY_HEAVY_ATT);
	m_Mgs->anim->createFromSheet(RES::PLY_HEAVY_ATT, RES::PLY_HEAVY_ATT, 14);

	m_Mgs->sprite->load(PLY_ASSETS "ply_attack3.bmp", RES::PLY_WHEEL_PUNCH);
	m_Mgs->anim->createFromSheet(RES::PLY_WHEEL_PUNCH, RES::PLY_WHEEL_PUNCH, 9);

	startAction(Actions::IDLE);
}

void Player::update(float a_Dt)
{
	m_IBuffer.update();

	Vector3& pos = m_Transform.pos;
	pos = getIPos();

	ActionData* currAct = m_Mgs->object->getAction(m_CurrentActKey);
	bool canInterrupt = (currAct == nullptr || currAct->interruptible);

	if (canInterrupt && m_Mgs->input->getInputDown(InputBind::ACT_X))
		m_AttackXRequested = true;
	if (canInterrupt && m_Mgs->input->getInputDown(InputBind::ACT_Y))
		m_AttackYRequested = true;
	if (canInterrupt && m_Mgs->input->getInputDown(InputBind::JUMP))
		m_JumpRequested = true;
}

Uint8 Player::getType() const
{
	return ObjectType::PLAYER;
}

void Player::draw()
{
	int activeAnimKey = getActiveAnim();
	float ply_w = (float)m_Mgs->anim->get(activeAnimKey)->frames[m_CurrentFrame].w;
	drawShadow(RES::SHADOW, ply_w, SHADOW_DIMS);

	m_Mgs->display->drawAnimFrame(activeAnimKey, m_CurrentFrame, m_Transform);
}

void Enemy::draw()
{
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

InputBuffer& Player::getIBuffer() 
{
	return m_IBuffer;
}

