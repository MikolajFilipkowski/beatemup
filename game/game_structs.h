#pragma once

#include "../core/core.h"
#include "config.h"

struct GameSettings {
	Uint32 windowWidth{ 0 };
	Uint32 windowHeight{ 0 };
	bool fullscreen{ false };
	bool borderless{ false };
	bool resizable{ false };
	Uint32 levels{ 0 };
};

struct MulStatus {
	Uint8 threshold;
	const char* text;
	float mul;
};

class GameState {
protected:
	ArrayList<MulStatus> m_Statuses{};
	char m_PlayerName[MAX_PLY_LEN + 1]{""};
	int m_Score{0};
	int m_Hits{ 0 };
	int m_EnemyCount{0};
	float m_ResetTimer{ 0.0f };
	float m_DisplayTimer{ 0.0f };
	MulStatus* m_CurrentStatus{ nullptr };
public:
	void addStatus(MulStatus a_Status);

	void tick(float a_FixedDt);
	void reset();
	void resetMul();
	void setPlayerName(const char* a_PlayerName);

	void addHit(float dmg, bool isContinuous);

	float getDisplayTimer() const;
	int getScore() const;
	const MulStatus* getCurrentStatus() const;
};