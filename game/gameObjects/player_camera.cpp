#include "player_camera.h"

#include "player.h"
#include <cstdio>

PlayerCamera::PlayerCamera(Managers* mgs, Player* ply, Vector3 pos) : Camera(mgs, pos), ply(ply) {

}

PlayerCamera::~PlayerCamera(){}

void PlayerCamera::update(float dt)
{
	assert(ply != nullptr && "Player is nullptr");

	transform.pos = getIPos();
}

void PlayerCamera::fixedUpdate(float fixed_dt)
{
	assert(ply != nullptr && "Player is nullptr");

	float plyX = ply->getPosition().x;
	float scrW = (float)mgs->display->getLogWidth();

	float plyCamDist = plyX - rb.currPos.x;

	float threshold = scrW * CAMERA_THR;

	rb.prevPos = rb.currPos;
	float targetX = rb.currPos.x;

	// jesli gracz daleko od srodka kamera podaza w jego strone
	if (plyCamDist > threshold) {
		targetX = plyX - threshold;
	}
	else if (plyCamDist < -threshold) {
		targetX = plyX + threshold;
	}

	float dx = targetX - rb.currPos.x;
	rb.currPos.x += dx * CAMERA_LERP;
}
