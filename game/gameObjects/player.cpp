#include "player.h"
#include <cstdio>
#include "enemy.h"
#include "../scenes/level_scene.h"

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
	case Actions::HURT:
		return RES::PLY_HURT;
	case Actions::DEATH:
		return RES::PLY_DEATH;
	case Actions::DASH_FORWARD:
	case Actions::DASH_BACKWARD:
		return RES::PLY_DASH;
	case Actions::LIGHT_ATTACK: 
		return RES::PLY_LIGHT_ATT;
	case Actions::HEAVY_ATTACK: 
		return RES::PLY_HEAVY_ATT;
	case Actions::WHEEL_PUNCH:
		return RES::PLY_WHEEL_PUNCH;
	case Actions::HIGH_KICK:
		return RES::PLY_HIGH_KICK;
	case Actions::JUMP_KICK:
		return RES::PLY_JUMP_KICK;
	case Actions::JUMP_PUNCH:
		return RES::PLY_JUMP_PUNCH;
	default: 
		return RES::NONE;
	}
}

void Player::computeInput()
{
	ActionData* currAct = getCurrAction();
	bool canInterrupt = (currAct == nullptr || currAct->interruptible);
	bool isMoving = false;

	// Zmiana stanu inputEntries z IN_USE na USED po animacji + delay
	if (m_BufferDecay > 0) {
		m_BufferDecay--;
		if (m_BufferDecay <= 0) m_IBuffer.flagUsed();
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

	if (isMoving) startAction(Actions::WALK);
	else startAction(Actions::IDLE);
}

Player::Player(Managers* a_Managers, GameState* a_GameState, Transform a_Transform)
	: Actor(a_Managers, a_GameState, a_Transform), m_IBuffer(a_Managers, this) {}

Player::~Player()
{
	unloadAnims();
}

void Player::start()
{
	m_Mgs->sprite->load(PLY_ASSETS "idle.bmp", getAnimFromAct(Actions::IDLE));
	m_Mgs->sprite->load(PLY_ASSETS "walk.bmp", getAnimFromAct(Actions::WALK));
	m_Mgs->sprite->load(PLY_ASSETS "jump.bmp", getAnimFromAct(Actions::JUMP));
	m_Mgs->sprite->load(PLY_ASSETS "hurt.bmp", getAnimFromAct(Actions::HURT));
	m_Mgs->sprite->load(PLY_ASSETS "death.bmp", getAnimFromAct(Actions::DEATH));
	m_Mgs->sprite->load(PLY_ASSETS "dash.bmp", getAnimFromAct(Actions::DASH_FORWARD));
	m_Mgs->sprite->load(PLY_ASSETS "attack1.bmp", getAnimFromAct(Actions::LIGHT_ATTACK));
	m_Mgs->sprite->load(PLY_ASSETS "attack2.bmp", getAnimFromAct(Actions::HEAVY_ATTACK));
	m_Mgs->sprite->load(PLY_ASSETS "attack3.bmp", getAnimFromAct(Actions::WHEEL_PUNCH));
	m_Mgs->sprite->load(PLY_ASSETS "high_kick.bmp", getAnimFromAct(Actions::HIGH_KICK));
	m_Mgs->sprite->load(PLY_ASSETS "jump_kick.bmp", getAnimFromAct(Actions::JUMP_KICK));
	m_Mgs->sprite->load(PLY_ASSETS "jump_punch.bmp", getAnimFromAct(Actions::JUMP_PUNCH));

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

void Player::hurt()
{
	m_GameState->resetMul();
}

void Player::die()
{
	if (m_Mgs->scene->getCurrentSceneIdx() == SceneID::LEVEL) {
		LevelScene* scene = (LevelScene*)m_Mgs->scene->getCurrentScene();
		scene->endModal(false);
	}
}

Uint8 Player::getType() const
{
	return ObjectType::PLAYER;
}

void Player::drawActionName()
{
	const char* actName = m_Mgs->object->getAction(m_CurrentActKey)->name;
	float logW = (float)m_Mgs->display->getLogWidth();

	float txtW = strlen(actName) * DEBUG_ACT_FONT.chSize * DEBUG_ACT_FONT.scale * DEBUG_ACT_FONT.spacing;

	float posX = logW / 2.0f - txtW / 2.0f;
	float posY = 50.0f;

	m_Mgs->display->drawString({ posX, posY }, actName, DEBUG_ACT_FONT);
}

void Player::drawPos()
{
	char buff[MAX_TEXTSIZE];

	Vector3& pos = m_Rb.currPos;
	snprintf(buff, (size_t)(MAX_TEXTSIZE - 1), "XYZ: %.3f / %.3f / %.3f", pos.x, pos.y, pos.z);

	float dy = INFO_FONT.chSize * INFO_FONT.scale;
	float posX = 5.0f;
	float posY = 5.0f + 2 * dy;

	m_Mgs->display->drawString({ posX, posY }, buff, INFO_FONT);
}

InputBuffer& Player::getIBuffer() 
{
	return m_IBuffer;
}

void Player::attackSuccess(float a_Dmg, bool isContinuous)
{
	m_GameState->addHit(a_Dmg, isContinuous);
}