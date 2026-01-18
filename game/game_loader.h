#pragma once

#include <cstdio>
#include "../core/core.h"
#include "config.h"
#include "game_structs.h"

struct Highscore {
	int score;
	char name[MAX_PLY_LEN + 1];
};

namespace LvlMask {
	enum Info : Uint8 {
		INFO_ENEMIES = 1 << 0,
		INFO_OBSTACLES = 1 << 1,
		INFO_ZONES = 1 << 2,
		INFO_WIDTH = 1 << 3,
		INFO_BACKGROUND = 1 << 4,
		INFO_ALL = 0x1F,
	};

	enum Enemy : Uint8 {
		ENEMY_TYPE = 1 << 0,
		ENEMY_POS = 1 << 1,
		ENEMY_ALL = 0x03,
	};

	enum Obstacle : Uint8 {
		OBSTACLE_TYPE = 1 << 0,
		OBSTACLE_POS = 1 << 1,
		OBSTACLE_DIMS = 1 << 2,
		OBSTACLE_MASS = 1 << 3,
		OBSTACLE_ALL = 0x0F,
	};
	enum Zone : Uint8 {
		ZONE_BEGINNING = 1 << 0,
		ZONE_END = 1 << 1,
		ZONE_FLAGS = 1 << 2,
		ZONE_ALL = 0x07,
	};
};

class GameLoader {
private:
	Managers* m_Mgs{};

	static constexpr int MAX_LINE_LEN = 512;

	static constexpr const char* FILE_NF_ERR = "File %s not found.\n";
	static constexpr const char* SETT_SYN_ERR = "File %s has incorrect syntax (line %d).\n";
	static constexpr const char* SETT_KEY_ERR = "Invalid key in %s (line %d)\n";

	static constexpr const char* HEADER_FORMAT = "[%d] %d %d %d %x %d %d %d %d %x";
	static constexpr int HEADER_VARS = 10;
	static constexpr const char* HEADER_ERR = "Action file has incorrect header data.\n";
	
	static constexpr const char* NAME_ERR = "Action name too long! Max 64 characters.\n";

	static constexpr const char* FR_FORMAT = "< %d > %f %f %f %f %f | %f %f %f %f | %f %f %f %f";
	static constexpr int FR_VARS = 14;
	static constexpr const char* FR_ERR = "Action %d has incorrect frame data. Frame line (relative to action): %d.\n";
	static constexpr const char* FR_COUNT_ERR = "Action %d has incorrect information in the header. Should have %d frames, loaded %d.\n";
	static constexpr const char* FR_ORDER_WARN = "WARNING: Frames are not in order. Frame %d created instead of %d!\n";

	static constexpr const char* LVL_NO_PRE_ERR = "File %s should have preambule data at the top of the file.\n";
	static constexpr const char* LVL_PREAMB_ERR = "File %s has incorrect preambule data.\n";

	static constexpr const char* LVL_ECOUNT_ERR = "File %s has %d enemies instead of %d.\n";
	static constexpr const char* LVL_OCOUNT_ERR = "File %s has %d obstacles instead of %d.\n";
	static constexpr const char* LVL_ZCOUNT_ERR = "File %s has %d zones instead of %d.\n";

	static constexpr const char* LVL_ENEMY_ERR = "File %s has incorrect enemy info.\n";
	static constexpr const char* LVL_OBSTACLE_ERR = "File %s has incorrect obstacle info.\n";
	static constexpr const char* LVL_ZONE_ERR = "File %s has incorrect zone info.\n";

	static int charToBind(const char& a_Char);
	void sortScores(Highscore* a_Scores, int a_LoadedCount) const;
	bool shouldSkip(char* a_Line) const;
public:
	GameLoader(Managers* a_Managers);
	bool loadGameSettings(const char* a_FilePath, GameSettings& a_Settings);
	bool parseSettLine(GameSettings& a_Settings, char* a_Key, char* a_Val);
	void loadActionData(const char* a_FilePath);
	bool parseHeader(char* a_Line, FILE* a_File, ActionData* a_Act, 
					int& a_Id, int& a_SeqLen, int& a_FrCount);
	void parseName(char* a_Line, ActionData* a_Act);
	Array<int>* parseSequence(const char* line, int seqLen);
	bool parseFrame(const char* a_Line, FILE* a_File, ActionFrame& a_Fr, int& a_FrLoaded, int& a_Id);

	void saveScore(GameState* a_State);
	Highscore* loadScores(int& a_LoadedCount) const;

	bool loadLevel(int a_Id, LoadedLevel& a_Level);
	bool parseLvlPreambule(FILE* a_File, LoadedLevel& a_Level, char* a_Buffer, char* a_FilePath);
	bool parseLvlEnemy(FILE* a_File, LoadedLevel& a_Level, Uint32& a_LoadedCount, char* a_Buffer, char* a_FilePath);
	bool parseLvlObstacle(FILE* a_File, LoadedLevel& a_Level, Uint32& a_LoadedCount, char* a_Buffer, char* a_FilePath);
	bool parseLvlZone(FILE* a_File, LoadedLevel& a_Level, Uint32& a_LoadedCount, char* a_Buffer, char* a_FilePath);
};