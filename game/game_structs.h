#pragma once

#include "../core/core.h"
#include "config.h"

int getObstacleSpriteKey(int a_ObstacleKey);

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

struct LoadedZone {
	Vector2 beginning{};
	Vector2 end{};
	Uint8 flags{ 0U };
};

struct LoadedEnemy {
	Uint8 type{ 0U };
	Vector3 pos{};
};

struct LoadedObstacle {
	Uint8 type{ 0U };
	Vector3 pos{};
	Vector3 dims{};
	float mass{ 0.0f };
};

struct LoadedLevel {
	int id{ 0 };
	LoadedEnemy* enemies{};
	Uint32 enemyCount{ 0 };
	LoadedObstacle* obstacles{};
	Uint32 obstacleCount{ 0 };
	LoadedZone* zones{};
	Uint32 zoneCount{ 0 };
	float width{ 0.0f };
	char* background{};

	LoadedLevel() = default;
	LoadedLevel(const LoadedLevel&) = delete;
	LoadedLevel& operator=(const LoadedLevel&) = delete;
	virtual ~LoadedLevel() {
		delete[] enemies;
		delete[] obstacles;
		delete[] zones;
		delete[] background;
	}
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
	const char* getPlayerName() const;
	void setPlayerName(const char* a_PlayerName);

	void addHit(float a_Dmg, bool a_IsContinuous);

	float getDisplayTimer() const;
	int getScore() const;
	void setScore(int a_Score);
	const MulStatus* getCurrentStatus() const;
};