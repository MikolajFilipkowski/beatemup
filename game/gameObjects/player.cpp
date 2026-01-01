#include "player.h"

Player::Player(Managers* mgs, Transform tr) : AnimatableObject(mgs, tr), jumpRequested(false) {}

Player::~Player()
{
}

void Player::start()
{
	mgs->sprite->load("game/assets/player/Idle.bmp", "player_idle");
	mgs->anim->createFromSheet("player_idle", "player_idle", 6, 0.2f);

	mgs->sprite->load("game/assets/player/Run.bmp", "player_run");
	mgs->anim->createFromSheet("player_run", "player_run", 8, 0.05f);

	setAnim("player_idle");
}

void Player::update(float dt)
{
	if (mgs->input->getAction("left")) {
		transform.flip = H_FLIP;
	}
	if (mgs->input->getAction("right")) {
		transform.flip = NO_FLIP;
	}

	bool isMoving = mgs->input->getAction("left") || mgs->input->getAction("right");

	if (isMoving) {
		setAnim("player_run");
	} else {
		setAnim("player_idle");
	}
	
	updateAnim(dt);
}

void Player::fixedUpdate(float fixed_dt)
{
	if (mgs->input->getAction("left")) {
		getPosition().x -= 7;
	}
	if (mgs->input->getAction("right")) {
		getPosition().x += 7;
	}
	if (mgs->input->getAction("jump")) {
		getRb().vel.y += 100;
	}
}

void Player::draw()
{
	mgs->display->drawAnimFrame(currentAnimKey, currentAnimFrame, transform);
}
