#include "player_camera.h"

#include "player.h"
#include <cstdio>

PlayerCamera::PlayerCamera(Managers* a_Managers, Player* a_Ply, Vector3 a_Pos) : Camera(a_Managers, a_Pos), m_Player(a_Ply) {

}

PlayerCamera::~PlayerCamera(){}

void PlayerCamera::update(float a_Dt)
{
	assert(m_Player != nullptr && "Player is nullptr");

	m_Transform.pos = getIPos();
}

void PlayerCamera::fixedUpdate(float a_FixedDt)
{
	assert(m_Player != nullptr && "Player is nullptr");

	float plyX = m_Player->getPosition().x;
	float scrW = (float)m_Mgs->display->getLogWidth();

	float plyCamDist = plyX - m_Rb.currPos.x;

	float threshold = scrW * CAMERA_THR;

	m_Rb.prevPos = m_Rb.currPos;
	float targetX = m_Rb.currPos.x;

	// jesli gracz daleko od srodka kamera podaza w jego strone
	if (plyCamDist > threshold) {
		targetX = plyX - threshold;
	}
	else if (plyCamDist < -threshold) {
		targetX = plyX + threshold;
	}

	float dx = targetX - m_Rb.currPos.x;
	m_Rb.currPos.x += dx * CAMERA_LERP;
}
