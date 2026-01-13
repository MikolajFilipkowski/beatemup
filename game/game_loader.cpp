#define _CRT_SECURE_NO_WARNINGS

#include "game_loader.h"

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

void GameLoader::init(Managers* a_Managers)
{
	m_Mgs = a_Managers;
}

void GameLoader::loadActionData(const char* a_FilePath)
{
	FILE* file = fopen(a_FilePath, "r");
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
		
		if (!fgets(line, MAX_LINE_LEN, file) || strlen(line) >= MAX_ACT_NAME_LEN) {
			printf(NAME_ERR);
			delete act;
			fclose(file);
			m_Mgs->engine->stop();
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
			printf(FR_COUNT_ERR, id, frCount, frLoaded);
			delete act;
			fclose(file);
			m_Mgs->engine->stop();
			return;
		}

		act->setFrames(frame);
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
		printf(HEADER_ERR);
		m_Mgs->engine->stop();
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
		printf(FR_ERR, a_Id, a_FrLoaded);
		m_Mgs->engine->stop();
		return false;
	}

	a_FrLoaded++;
	return true;
}
