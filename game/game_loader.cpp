#define _CRT_SECURE_NO_WARNINGS

#include "game_loader.h"
#include "gameObjects/doyle.h"
#include "gameObjects/autumn.h"
#include <cstring>

static constexpr const char* SCORES_PATH = "game/data/scores.dat";
static constexpr const char* LEVEL_PATH = "game/data/level_%d.cfg";

int GameLoader::charToBind(const char& a_Char)
{
	switch (a_Char) {
	case 'F': return ActionBind::FORWARD;
	case 'B': return ActionBind::BACKWARD;
	case 'U': return ActionBind::UP;
	case 'D': return ActionBind::DOWN;
	case 'J': return ActionBind::JUMP;
	case 'X': return ActionBind::ACT_X;
	case 'Y': return ActionBind::ACT_Y;
	default: return ActionBind::NONE;
	}
}

void GameLoader::sortScores(Highscore* a_Scores, int a_LoadedCount) const
{
	for (int i = 0; i < a_LoadedCount; i++) {
		bool swapped = false;

		for (int j = 0; j < a_LoadedCount - i - 1; j++) {
			if (a_Scores[j].score < a_Scores[j + 1].score) {
				Highscore temp = a_Scores[j];
				a_Scores[j] = a_Scores[j + 1];
				a_Scores[j + 1] = temp;
				swapped = true;
			}
		}

		if (!swapped) break;
	}
}

bool GameLoader::shouldSkip(char* a_Line) const
{
	return (a_Line[0] == '#' || a_Line[0] == ';' || a_Line[0] == '\n' || a_Line[0] == '\r');
}

GameLoader::GameLoader(Managers* a_Managers)
{
	m_Mgs = a_Managers;
}

bool GameLoader::loadGameSettings(const char* a_FilePath, GameSettings& a_Settings)
{
	FILE* file = fopen(a_FilePath, "r");
	if (!file) {
		m_Mgs->engine->throwError(FILE_NF_ERR, a_FilePath);
		return false;
	}

	char line[MAX_LINE_LEN];
	int lineCounter = 1;

	while (fgets(line, sizeof(line), file)) {
		if (shouldSkip(line)) continue;

		char* key = strtok(line, "=");
		char* val = strtok(NULL, "\n");

		if (key == NULL || val == NULL) {
			m_Mgs->engine->throwError(SETT_SYN_ERR, a_FilePath, lineCounter);
			fclose(file);
			return false;
		}

		if (!strcmp(key, "window_width"))
			a_Settings.windowWidth = strtoul(val, NULL, 10);
		else if (!strcmp(key, "window_height"))
			a_Settings.windowHeight = strtoul(val, NULL, 10);
		else if (!strcmp(key, "fullscreen"))
			a_Settings.fullscreen = !(strtoul(val, NULL, 10) == 0U);
		else if (!strcmp(key, "borderless"))
			a_Settings.borderless = !(strtoul(val, NULL, 10) == 0U);
		else if (!strcmp(key, "resizable"))
			a_Settings.resizable = !(strtoul(val, NULL, 10) == 0U);
		else if (!strcmp(key, "levels"))
			a_Settings.levels = strtoul(val, NULL, 10);
		else {
			m_Mgs->engine->throwError(SETT_KEY_ERR, a_FilePath, lineCounter);
			fclose(file);
			return false;
		}

		lineCounter++;
	}
	fclose(file);

	// Sprawdzenie czy wszystkie etapy istnieja
	Uint32 i = 1;
	for (; i <= a_Settings.levels; i++) {
		if(snprintf(line, (size_t)MAX_LINE_LEN - 1,  LEVEL_PATH, i) == 0) break;
		FILE* level = fopen(line, "r");
		if (!level) break;
		fclose(level);
	}

	if (i - 1 != a_Settings.levels) {
		m_Mgs->engine->throwError(FILE_NF_ERR, line);
		return false;
	}
	return true;
}

void GameLoader::loadActionData(const char* a_FilePath)
{
	FILE* file = fopen(a_FilePath, "r");
	if (!file) {
		m_Mgs->engine->throwError(FILE_NF_ERR, a_FilePath);
		return;
	}

	char line[MAX_LINE_LEN];

	while (fgets(line, sizeof(line), file)) {
		if (shouldSkip(line)) continue;

		int id, seqLen, frCount;
		ActionData* act = new ActionData();
		bool success = parseHeader(line, file, act, id, seqLen, frCount);

		if (!success) {
			fclose(file);
			return;
		}
		
		if (!fgets(line, MAX_LINE_LEN, file) || strlen(line) >= MAX_ACT_NAME_LEN) {
			m_Mgs->engine->throwError(NAME_ERR);
			delete act;
			fclose(file);
			return;
		}

		parseName(line, act);
		fgets(line, MAX_LINE_LEN, file);

		if (seqLen > 0)
			act->setSequence(parseSequence(line, seqLen));

		auto frame = new Array<ActionFrame>(frCount);

		int frLoaded = 0;
		while (frLoaded < frCount && fgets(line, sizeof(line), file)) {
			if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') 
				continue;
			
			bool success = parseFrame(line, file, frame->get(frLoaded), frLoaded, id);
			if (!success) {
				delete act;
				fclose(file);
				return;
			}
		}

		if (frLoaded < frCount) {
			m_Mgs->engine->throwError(FR_COUNT_ERR, id, frCount, frLoaded);
			delete act;
			fclose(file);
			return;
		}

		act->setFrames(frame);
		act->sealData();
		m_Mgs->object->addAction(id, act);
	}

	fclose(file);
}

bool GameLoader::parseHeader(char* a_Line, FILE* a_File, ActionData* a_Act, 
							int& a_Id, int& a_SeqLen, int& a_FrCount)
{
	int inter, res, owner, cond, canMv, shLoop;
	res = sscanf(a_Line, HEADER_FORMAT, &a_Id, &a_Act->priority, &a_Act->inputWindow, 
		&cond, &inter, &a_SeqLen, &a_FrCount, &canMv, &shLoop, &owner);

	a_Act->interruptible = (inter != 0);
	a_Act->canMove = (canMv != 0);
	a_Act->shouldLoop = (shLoop != 0);
	a_Act->owner = (owner >= 0 && owner <= SDL_MAX_UINT8) ? (Uint8)owner : 0;
	a_Act->conditions = (cond >= 0 && cond <= SDL_MAX_UINT8) ? (Uint8)cond : 0;

	// Jesli niepoprawne formatowanie rzuc blad
	if (res != HEADER_VARS) {
		m_Mgs->engine->throwError(HEADER_ERR);
		return false;
	}
	return true;
}

void GameLoader::parseName(char* a_Line, ActionData* a_Act)
{
	strncpy(a_Act->name, a_Line, MAX_ACT_NAME_LEN - 1);

	size_t actLen = strlen(a_Act->name);
	if (a_Act->name[actLen - 1] == '\n')
		a_Act->name[actLen - 1] = '\0';
}

Array<int>* GameLoader::parseSequence(const char* a_Line, int a_SeqLen)
{
	Array<int>* sequence = new Array<int>(a_SeqLen);

	int found = 0;

	for (int i = 0; a_Line[i] != '\0' && found < a_SeqLen; i++) {
		char ch = a_Line[i];

		int bind = charToBind(ch);
		if (bind != ActionBind::NONE)
			sequence->get(found++) = bind;
	}

	return sequence;
}

bool GameLoader::parseFrame(const char* a_Line, FILE* a_File, ActionFrame& a_Fr, int& a_FrLoaded, int& a_Id)
{
	int fId;

	int res = sscanf(a_Line, FR_FORMAT,
		&fId, &a_Fr.duration, &a_Fr.vel.x, &a_Fr.vel.y, &a_Fr.vel.z, &a_Fr.damage,
		&a_Fr.hitbox.x, &a_Fr.hitbox.y, &a_Fr.hitbox.w, &a_Fr.hitbox.h,
		&a_Fr.hurtbox.x, &a_Fr.hurtbox.y, &a_Fr.hurtbox.w, &a_Fr.hurtbox.h
	);

	if (fId != a_FrLoaded)
		printf(FR_ORDER_WARN, fId, a_FrLoaded);

	if (res != FR_VARS) {
		m_Mgs->engine->throwError(FR_ERR, a_Id, a_FrLoaded);
		return false;
	}

	a_FrLoaded++;
	return true;
}


void GameLoader::saveScore(GameState* a_State)
{
	Highscore hs = { .score = a_State->getScore() };

	const char* name = a_State->getPlayerName();
	if (strcmp(name, "") == 0)
		name = "Anonym";

	strncpy(hs.name, name, MAX_PLY_LEN);
	hs.name[MAX_PLY_LEN] = '\0';

	FILE* file = fopen(SCORES_PATH, "ab");
	if (!file) {
		printf("Cannot access file: %s\n", SCORES_PATH);
		return;
	}
	fwrite(&hs, sizeof(hs), 1, file);

	fclose(file);
}

Highscore* GameLoader::loadScores(int& a_LoadedCount) const
{
	FILE* file = fopen(SCORES_PATH, "rb");
	if (!file) {
		printf("Cannot access file: %s\n", SCORES_PATH);
		return nullptr;
	}

	if (fseek(file, 0, SEEK_END) != 0) {
		printf("Cannot read data from file: %s\n", SCORES_PATH);
		fclose(file);
		return nullptr;
	}

	int scoreCount = (int)(ftell(file) / sizeof(Highscore));
	rewind(file);

	if (scoreCount <= 0) {
		a_LoadedCount = 0;
		fclose(file);
		return nullptr;
	}

	Highscore* highscores = new Highscore[scoreCount];
	fread(highscores, sizeof(Highscore), scoreCount, file);
	a_LoadedCount = scoreCount;

	sortScores(highscores, scoreCount);

	fclose(file);

	return highscores;
}

bool GameLoader::loadLevel(int a_Id, LoadedLevel& a_Level)
{
	a_Level.id = a_Id;
	char buff[MAX_LINE_LEN]{};
	bool preambuleLoaded = false;

	snprintf(buff, (size_t)MAX_LINE_LEN - 1, LEVEL_PATH, a_Id);
	char fpath[MAX_LINE_LEN];
	strncpy(fpath, buff, MAX_LINE_LEN);

	FILE* file = fopen(buff, "r");

	if (!file) {
		m_Mgs->engine->throwError(FILE_NF_ERR, buff);
		return false;
	}

	Uint32 enemiesC = 0;
	Uint32 obstaclesC = 0;
	Uint32 zonesC = 0;
	bool success = true;

	while (fgets(buff, MAX_LINE_LEN - 1, file)) {
		if (shouldSkip(buff)) continue;

		if (strstr(buff, "[INFO]")) {
			preambuleLoaded = parseLvlPreambule(file, a_Level, buff, fpath);
			if (!preambuleLoaded) return false;
			a_Level.enemies = new LoadedEnemy[a_Level.enemyCount];
			a_Level.obstacles = new LoadedObstacle[a_Level.obstacleCount];
			a_Level.zones = new LoadedZone[a_Level.zoneCount];
			continue;
		}
		else if (!preambuleLoaded) {
			m_Mgs->engine->throwError(LVL_NO_PRE_ERR, fpath);
			fclose(file);
			return false;
		}

		if (strstr(buff, "[ENEMY]")) {
			if (enemiesC >= a_Level.enemyCount) {
				m_Mgs->engine->throwError(LVL_ECOUNT_ERR, fpath, enemiesC + 1, a_Level.enemyCount);
				fclose(file);
				return false;
			}
			success = parseLvlEnemy(file, a_Level, enemiesC, buff, fpath);
			if (!success) return false;
			continue;
		} else if (strstr(buff, "[OBSTACLE]")) {
			if (obstaclesC >= a_Level.obstacleCount) {
				m_Mgs->engine->throwError(LVL_OCOUNT_ERR, fpath, obstaclesC + 1, a_Level.obstacleCount);
				fclose(file);
				return false;
			}
			success = parseLvlObstacle(file, a_Level, obstaclesC, buff, fpath);
			if (!success) return false;
			continue;
		} else if (strstr(buff, "[ZONE]")) {
			if (zonesC >= a_Level.zoneCount) {
				m_Mgs->engine->throwError(LVL_ZCOUNT_ERR, fpath, zonesC + 1, a_Level.zoneCount);
				fclose(file);
				return false;
			}
			success = parseLvlZone(file, a_Level, zonesC, buff, fpath);
			if (!success) return false;
			continue;
		}
	}

	if (enemiesC < a_Level.enemyCount) {
		m_Mgs->engine->throwError(LVL_ECOUNT_ERR, fpath, enemiesC, a_Level.enemyCount);
		fclose(file);
		return false;
	}
	if (obstaclesC < a_Level.obstacleCount) {
		m_Mgs->engine->throwError(LVL_OCOUNT_ERR, fpath, obstaclesC, a_Level.obstacleCount);
		fclose(file);
		return false;
	}
	if (zonesC < a_Level.zoneCount) {
		m_Mgs->engine->throwError(LVL_ZCOUNT_ERR, fpath, zonesC, a_Level.zoneCount);
		fclose(file);
		return false;
	}

	fclose(file);
	return true;
}

bool GameLoader::parseLvlPreambule(FILE* a_File, LoadedLevel& a_Level, char* a_Buffer, char* a_FilePath)
{
	char *key, *value;
	Uint8 mask = 0;

	while (mask != LvlMask::INFO_ALL && fgets(a_Buffer, MAX_LINE_LEN - 1, a_File)) {
		if (shouldSkip(a_Buffer)) continue;
		key = strtok(a_Buffer, "=");
		value = strtok(NULL, "\n");

		if (key == NULL || value == NULL) break;

		if (strstr(key, "enemies")) {
			a_Level.enemyCount = strtoul(value, NULL, 10);
			mask |= LvlMask::INFO_ENEMIES;
		}
		else if (strstr(key, "obstacles")) {
			a_Level.obstacleCount = strtoul(value, NULL, 10);
			mask |= LvlMask::INFO_OBSTACLES;
		}
		else if (strstr(key, "zones")) {
			a_Level.zoneCount = strtoul(value, NULL, 10);
			mask |= LvlMask::INFO_ZONES;
		}
		else if (strstr(key, "width")) {
			a_Level.width = strtof(value, NULL);
			mask |= LvlMask::INFO_WIDTH;
		}
		else if (strstr(key, "background")) {
			a_Level.background = copy_string(ltrim(value));
			mask |= LvlMask::INFO_BACKGROUND;
		}
		else break;
	}
	
	if ((mask & LvlMask::INFO_ALL) == LvlMask::INFO_ALL)
		return true;

	m_Mgs->engine->throwError(LVL_PREAMB_ERR, a_FilePath);
	fclose(a_File);
	return false;
}

bool GameLoader::parseLvlEnemy(FILE* a_File, LoadedLevel& a_Level, Uint32& a_LoadedCount, char* a_Buffer, char* a_FilePath)
{
	char *key, *value;
	Uint8 mask = 0;
	int type;
	Vector3 pos;

	while (mask != LvlMask::ENEMY_ALL && fgets(a_Buffer, MAX_LINE_LEN - 1, a_File)) {
		if (shouldSkip(a_Buffer)) continue;
		key = strtok(a_Buffer, "=");
		value = strtok(NULL, "\n");

		if (key == NULL || value == NULL) break;

		if (strstr(key, "type")) {
			type = strtoul(value, NULL, 10);
			mask |= LvlMask::ENEMY_TYPE;
		}
		else if (strstr(key, "pos")) {
			int loaded = sscanf(value, " %f %f %f ", &pos.x, &pos.y, &pos.z);
			if (loaded != 3) break;
			mask |= LvlMask::ENEMY_POS;
		}
		else break;
	}

	if ((mask & LvlMask::ENEMY_ALL) == LvlMask::ENEMY_ALL) {
		a_Level.enemies[a_LoadedCount].type = type;
		a_Level.enemies[a_LoadedCount].pos = pos;
		a_LoadedCount++;
		return true;
	}

	m_Mgs->engine->throwError(LVL_ENEMY_ERR, a_FilePath);
	fclose(a_File);
	return false;
}

bool GameLoader::parseLvlObstacle(FILE* a_File, LoadedLevel& a_Level, Uint32& a_LoadedCount, char* a_Buffer, char* a_FilePath)
{
	char* key, * value;
	Uint8 mask = 0;
	int type;
	float mass = 0.0f;
	Vector3 pos, dims;

	while (mask != LvlMask::OBSTACLE_ALL && fgets(a_Buffer, MAX_LINE_LEN - 1, a_File)) {
		if (shouldSkip(a_Buffer)) continue;
		key = strtok(a_Buffer, "=");
		value = strtok(NULL, "\n");

		if (key == NULL || value == NULL) break;

		if (strstr(key, "type")) {
			type = strtoul(value, NULL, 10);
			mask |= LvlMask::OBSTACLE_TYPE;
		}
		else if (strstr(key, "pos")) {
			int loaded = sscanf(value, " %f %f %f ", &pos.x, &pos.y, &pos.z);
			if (loaded != 3) break;
			mask |= LvlMask::OBSTACLE_POS;
		}
		else if (strstr(key, "dims")) {
			int loaded = sscanf(value, " %f %f %f ", &dims.x, &dims.y, &dims.z);
			if (loaded != 3) break;
			mask |= LvlMask::OBSTACLE_DIMS;
		}
		else if (strstr(key, "mass")) {
			mass = strtof(value, NULL);
			mask |= LvlMask::OBSTACLE_MASS;
		}
		else break;
	}

	if ((mask & LvlMask::OBSTACLE_ALL) == LvlMask::OBSTACLE_ALL) {
		a_Level.obstacles[a_LoadedCount].type = type;
		a_Level.obstacles[a_LoadedCount].pos = pos;
		a_Level.obstacles[a_LoadedCount].dims = dims;
		a_Level.obstacles[a_LoadedCount].mass = mass;
		a_LoadedCount++;
		return true;
	}

	m_Mgs->engine->throwError(LVL_OBSTACLE_ERR, a_FilePath);
	fclose(a_File);
	return false;
}

bool GameLoader::parseLvlZone(FILE* a_File, LoadedLevel& a_Level, Uint32& a_LoadedCount, char* a_Buffer, char* a_FilePath)
{
	char* key, * value;
	Vector2 beginning, end;
	Uint8 flags, mask = 0;

	while (mask != LvlMask::ZONE_ALL && fgets(a_Buffer, MAX_LINE_LEN - 1, a_File)) {
		if (shouldSkip(a_Buffer)) continue;
		key = strtok(a_Buffer, "=");
		value = strtok(NULL, "\n");

		if (key == NULL || value == NULL) break;

		if (strstr(key, "beginning")) {
			int loaded = sscanf(value, " %f %f ", &beginning.x, &beginning.y);
			if (loaded != 2) break;
			mask |= LvlMask::ZONE_BEGINNING;
		}
		else if (strstr(key, "end")) {
			int loaded = sscanf(value, " %f %f ", &end.x, &end.y);
			if (loaded != 2) break;
			mask |= LvlMask::ZONE_END;
		}
		else if (strstr(key, "flags")) {
			flags = (Uint8)strtoul(value, NULL, 16);
			mask |= LvlMask::ZONE_FLAGS;
		}
		else break;
	}

	if ((mask & LvlMask::ZONE_ALL) == LvlMask::ZONE_ALL) {
		a_Level.zones[a_LoadedCount].beginning = beginning;
		a_Level.zones[a_LoadedCount].end = end;
		a_Level.zones[a_LoadedCount].flags = flags;
		a_LoadedCount++;
		return true;
	}

	m_Mgs->engine->throwError(LVL_ZONE_ERR, a_FilePath);
	fclose(a_File);
	return false;
}
