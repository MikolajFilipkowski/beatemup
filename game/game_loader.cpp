#define _CRT_SECURE_NO_WARNINGS

#include "game_loader.h"

int GameLoader::charToBind(const char& ch)
{
	switch (ch) {
	case 'L': return ActionBind::LEFT;
	case 'R': return ActionBind::RIGHT;
	case 'U': return ActionBind::UP;
	case 'D': return ActionBind::DOWN;
	case 'J': return ActionBind::JUMP;
	case 'X': return ActionBind::ACT_X;
	case 'Y': return ActionBind::ACT_Y;
	default: return ActionBind::NONE;
	}
}

void GameLoader::loadActionData(const char* filePath)
{
	FILE* file = fopen(filePath, "r");
	if (!file) return;

	char line[MAX_LINE_LEN];

	while (fgets(line, sizeof(line), file)) {
		if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

		int id, seqLen, frCount;
		ActionData* act = new ActionData();
		bool success = parseHeader(line, file, act, id, seqLen, frCount);

		if (!success) {
			fclose(file);
			return;
		}

		act->sequence = (seqLen > 0) ? parseSequence(line, seqLen) : nullptr;
		act->frames = new Array<ActionFrame>(frCount);

		int frLoaded = 0;
		while (frLoaded < frCount && fgets(line, sizeof(line), file)) {
			if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') 
				continue;
			
			bool success = parseFrame(line, file, act->frames->get(frLoaded), frLoaded, id);
			if (!success) {
				delete act;
				fclose(file);
				return;
			}
		}

		if (frLoaded < frCount) {
			printf(FR_COUNT_ERR, id, frCount, frLoaded);
			delete act;
			fclose(file);
			mgs->engine->stop();
			return;
		}

		mgs->object->addAction(id, act);
	}

	fclose(file);
}

bool GameLoader::parseHeader(char* line, FILE* file, ActionData* act, 
							int& id, int& seqLen, int& frCount)
{
	int inter, res;
	res = sscanf(line, HEADER_FORMAT, &id, &act->priority, &act->inputWindow, 
		&act->conditions, &inter, &seqLen, &frCount, &act->owner);

	act->interrupt = (inter != 0);

	// Jesli niepoprawne formatowanie lub kolejna linia nie istnieje rzuc blad
	if (res != HEADER_VARS || !fgets(line, sizeof(line), file)) {
		printf(HEADER_ERR);
		mgs->engine->stop();
		return false;
	}
	return true;
}

Array<int>* GameLoader::parseSequence(const char* line, int seqLen)
{
	Array<int>* sequence = new Array<int>(seqLen);

	int found = 0;

	for (int i = 0; line[i] != '\0' && found < seqLen; i++) {
		char ch = line[i];

		int bind = charToBind(ch);
		if (bind != ActionBind::NONE)
			sequence->get(found++) = bind;
	}

	return sequence;
}

bool GameLoader::parseFrame(const char* line, FILE* file, ActionFrame& fr, int& frLoaded, int& id)
{
	int fId;

	int res = sscanf(line, FR_FORMAT,
		&fId, &fr.duration, &fr.vel.x, &fr.vel.y, &fr.vel.z, &fr.damage,
		&fr.hitbox.x, &fr.hitbox.y, &fr.hitbox.w, &fr.hitbox.h,
		&fr.hurtbox.x, &fr.hurtbox.y, &fr.hurtbox.w, &fr.hurtbox.h
	);

	if (fId != frLoaded)
		printf(FR_ORDER_WARN, fId, frLoaded);

	if (res != FR_VARS) {
		printf(FR_ERR, id, frLoaded);
		mgs->engine->stop();
		return false;
	}

	frLoaded++;
	return true;
}
