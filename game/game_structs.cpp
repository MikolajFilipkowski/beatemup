#define _CRT_SECURE_NO_WARNINGS

#include "game_structs.h"

int getObstacleSpriteKey(int a_ObstacleKey) {
	switch (a_ObstacleKey) {
	case ObstacleType::BARREL:
		return RES::BARREL;
	default:
		return RES::NONE;
	}
	
}

void GameState::addStatus(MulStatus a_Status)
{
	int i = 0;
	for (i = 0; i < m_Statuses.count(); i++) {
		if (a_Status.threshold > m_Statuses[i].threshold)
			break;
	}
	m_Statuses.insert(i, a_Status);
}

void GameState::tick(float a_FixedDt) {
	if (m_ResetTimer > 0.0f) {
		m_ResetTimer -= a_FixedDt;

		if (m_ResetTimer <= 0.0f) 
			resetMul();
	}

	if (m_DisplayTimer > 0.0f) {
		m_DisplayTimer -= a_FixedDt;

		if (m_DisplayTimer <= 0.0f)
			m_DisplayTimer = 0.0f;
	}
}

void GameState::reset() {
	m_Score = 0;
	m_Hits = 0;
	m_EnemyCount = 0;
	m_ResetTimer = 0.0f;
	m_CurrentStatus = nullptr;
}

void GameState::resetMul() {
	m_Hits = 0;
	m_ResetTimer = 0.0f;
	m_CurrentStatus = nullptr;
}

const char* GameState::getPlayerName() const
{
	return m_PlayerName;
}

void GameState::setPlayerName(const char* a_PlayerName)
{
	strncpy(m_PlayerName, a_PlayerName, MAX_PLY_LEN);
	m_PlayerName[MAX_PLY_LEN] = '\0';
}

void GameState::addHit(float a_Dmg, bool a_IsContinuous) {
	if (!a_IsContinuous)
		m_Hits++;

	m_ResetTimer = COMBO_DECAY;
	float hits_mul = 1.0f + m_Hits * HITS_MUL;

	for (auto& status : m_Statuses) {
		if (status.threshold == m_Hits && !a_IsContinuous)
			m_DisplayTimer = COMBO_DISPLAY_TIME;
		if (status.threshold <= m_Hits) {
			m_CurrentStatus = &status;
			m_Score += (int)(a_Dmg * BASE_SCORE_MUL * status.mul * hits_mul);
			return;
		}
	}

	m_CurrentStatus = nullptr;
	m_Score += (int)(a_Dmg * BASE_SCORE_MUL);
}

float GameState::getDisplayTimer() const
{
	return m_DisplayTimer;
}

int GameState::getScore() const
{
	return m_Score;
}

void GameState::setScore(int a_Score)
{
	m_Score = a_Score;
}

const MulStatus* GameState::getCurrentStatus() const
{
	return m_CurrentStatus;
}

