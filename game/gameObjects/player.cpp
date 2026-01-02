#include "player.h"
#include <cstdio>

Player::Player(Managers* mgs, Camera* cam, Transform tr) : AnimatableObject(mgs, tr), plyCam(cam), jumpRequested(false), isGrounded(true) {}

Player::~Player()
{
}

void Player::start()
{
	mgs->sprite->load("game/assets/player/Idle.bmp", RES::PLY_IDLE);
	mgs->anim->createFromSheet(RES::PLY_IDLE, RES::PLY_IDLE, 6, 0.2f);

	mgs->sprite->load("game/assets/player/Run.bmp", RES::PLY_RUN);
	mgs->anim->createFromSheet(RES::PLY_RUN, RES::PLY_RUN, 8, 0.05f);

	mgs->sprite->load("game/assets/player/Walk.bmp", RES::PLY_WALK);
	mgs->anim->createFromSheet(RES::PLY_WALK, RES::PLY_WALK, 8, 0.3f);

	mgs->sprite->load("game/assets/player/Jump.bmp", RES::PLY_JUMP);
	mgs->anim->createFromSheet(RES::PLY_JUMP, RES::PLY_JUMP, 10, 0.05f);

	setAnim(RES::PLY_IDLE);
}

void Player::update(float dt)
{
	Vector3& pos = transform.pos;

	if (mgs->input->getAction(Action::LEFT)) {
		transform.flip = H_FLIP;
	}
	if (mgs->input->getAction(Action::RIGHT)) {
		transform.flip = NO_FLIP;
	}

	if (mgs->input->getAction(Action::LEFT)) {
		pos.x -= 700 * dt;
	}
	if (mgs->input->getAction(Action::RIGHT)) {
		pos.x += 700 * dt;
	}
	if (mgs->input->getAction(Action::UP)) {
		pos.z = clamp(pos.z - (300 * dt), 200.0f, 350.0f);
	}
	if (mgs->input->getAction(Action::DOWN)) {
		pos.z = clamp(pos.z + (300 * dt), 200.0f, 350.0f);
	}
	if (mgs->input->getActionDown(Action::JUMP) && isGrounded) {
		jumpRequested = true;
	}

	bool isWalking = mgs->input->getAction(Action::UP) || mgs->input->getAction(Action::DOWN);
	bool isMoving = mgs->input->getAction(Action::LEFT) || mgs->input->getAction(Action::RIGHT);

	if (isGrounded) {
		if (isMoving) {
			setAnim(RES::PLY_RUN);
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
	
	plyCam->setPosition({ pos.x, 0, 500.0f });

	updateAnim(dt);
}

void Player::fixedUpdate(float fixed_dt)
{
	Vector3& pos = transform.pos;

	if (jumpRequested && isGrounded) {
		rb.vel.y = -600.0f;
		jumpRequested = false;
		isGrounded = false;
	}

	if (!isGrounded) {
		rb.vel.y += 20.0f;
	}

	printf("%.2f %.2f %.2f\n", pos.x, pos.y, pos.z);

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
	mgs->display->drawAnimFrame(currentAnimKey, currentAnimFrame, transform);
}
