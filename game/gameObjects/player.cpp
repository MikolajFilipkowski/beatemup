#include "player.h"
#include <cstdio>

#define PLY_ASSETS "game/assets/player/"

Player::Player(Managers* mgs, Transform tr) 
	: AnimatableObject(mgs, tr), jumpRequested(false), 
	isGrounded(true), isAttacking(false), attackTimer(0.0f) {}

Player::~Player()
{
}

void Player::start()
{
	mgs->sprite->load(PLY_ASSETS "ply_idle.bmp", RES::PLY_IDLE);
	mgs->anim->createFromSheet(RES::PLY_IDLE, RES::PLY_IDLE, 7, 0.1f);

	mgs->sprite->load(PLY_ASSETS "ply_walk.bmp", RES::PLY_WALK);
	mgs->anim->createFromSheet(RES::PLY_WALK, RES::PLY_WALK, 13, 0.1f);

	mgs->sprite->load(PLY_ASSETS "ply_jump.bmp", RES::PLY_JUMP);
	mgs->anim->createFromSheet(RES::PLY_JUMP, RES::PLY_JUMP, 13, 0.06f);

	mgs->sprite->load(PLY_ASSETS "ply_attack1.bmp", RES::PLY_ATTACK_1);
	mgs->anim->createFromSheet(RES::PLY_ATTACK_1, RES::PLY_ATTACK_1, 12, 0.03f);

	mgs->sprite->load(PLY_ASSETS "ply_attack2.bmp", RES::PLY_ATTACK_2);
	mgs->anim->createFromSheet(RES::PLY_ATTACK_2, RES::PLY_ATTACK_2, 14, 0.03f);

	mgs->sprite->load(PLY_ASSETS "ply_attack3.bmp", RES::PLY_ATTACK_3);
	mgs->anim->createFromSheet(RES::PLY_ATTACK_3, RES::PLY_ATTACK_3, 11, 0.03f);

	setAnim(RES::PLY_IDLE);
}

void Player::update(float dt)
{
	Vector3& pos = transform.pos;
	pos = getIPos();

	//printf("%.2f %.2f %.2f\n", pos.x, pos.y, pos.z);

	if (isAttacking) {
		attackTimer -= dt;
		if (attackTimer <= 0) {
			attackTimer = 0;
			isAttacking = false;
			setAnim(RES::PLY_IDLE);
		}
	}

	if (mgs->input->getAction(Action::LEFT)) {
		transform.flip = H_FLIP;
	}
	if (mgs->input->getAction(Action::RIGHT)) {
		transform.flip = NO_FLIP;
	}
	
	if (mgs->input->getAction(Action::JUMP) && isGrounded) {
		jumpRequested = true;
	}

	if (mgs->input->getActionDown(Action::ACT_X) && isGrounded && !jumpRequested) {
		isAttacking = true;
		AnimationClip* anim = mgs->anim->get(RES::PLY_ATTACK_2);
		attackTimer = anim->frameDuration * anim->frameCount;
		setAnim(RES::PLY_ATTACK_2);
	}
	else if (mgs->input->getActionDown(Action::ACT_Y) && isGrounded && !jumpRequested) {
		isAttacking = true;
		AnimationClip* anim = mgs->anim->get(RES::PLY_ATTACK_1);
		attackTimer = anim->frameDuration * anim->frameCount;
		setAnim(RES::PLY_ATTACK_1);
	}

	bool isWalking = mgs->input->getAction(Action::UP) || mgs->input->getAction(Action::DOWN);
	bool isMoving = mgs->input->getAction(Action::LEFT) || mgs->input->getAction(Action::RIGHT);

	if (!isAttacking) {
		if (isGrounded) {
			if (isMoving) {
				setAnim(RES::PLY_WALK);
			}
			else if (isWalking) {
				setAnim(RES::PLY_WALK);
			}
			else {
				setAnim(RES::PLY_IDLE);
			}
		}
		else {
			setAnim(RES::PLY_JUMP);
		}
	}

	updateAnim(dt);
}

void Player::fixedUpdate(float fixed_dt)
{
	rb.prevPos = rb.currPos;
	Vector3& pos = rb.currPos;

	if (mgs->input->getAction(Action::LEFT)) {
		pos.x -= 6;
	}
	if (mgs->input->getAction(Action::RIGHT)) {
		pos.x += 6;
	}
	if (mgs->input->getAction(Action::UP)) {
		pos.z = clamp(pos.z - 3, 220.0f, 400.0f);
	}
	if (mgs->input->getAction(Action::DOWN)) {
		pos.z = clamp(pos.z + 3, 220.0f, 400.0f);
	}

	if (jumpRequested && isGrounded) {
		rb.vel.y = -2000.0f;
		jumpRequested = false;
		isGrounded = false;
	}

	if (!isGrounded) {
		rb.vel.y += 50.0f;
	}

	pos.x += rb.vel.x * fixed_dt;
	pos.y += rb.vel.y * fixed_dt;
	pos.z += rb.vel.z * fixed_dt;

	if (pos.y >= 0) {
		pos.y = 0;
		rb.vel.y = 0;
		isGrounded = true;
	}
}

void Player::draw()
{
	float ply_w = (float)mgs->anim->get(currentAnimKey)->frames[currentAnimFrame].w;
	drawShadow(RES::SHADOW, ply_w, { 96, 32 });
	mgs->display->drawAnimFrame(currentAnimKey, currentAnimFrame, transform);
}
