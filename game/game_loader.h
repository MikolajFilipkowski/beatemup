#pragma once

#include <cstdio>
#include "../core/core.h"
#include "config.h"

class GameLoader {
private:
	Managers* mgs;

	static constexpr int MAX_LINE_LEN = 512;

	static constexpr const char* HEADER_FORMAT = "[%d] %d %d %d %d %d %d %x";
	static constexpr int HEADER_VARS = 8;
	static constexpr const char* HEADER_ERR = "Action file has incorrect header data.\n";

	static constexpr const char* FR_FORMAT = "< %d > %f %f %f %f %f | %f %f %f %f | %f %f %f %f";
	static constexpr int FR_VARS = 14;
	static constexpr const char* FR_ERR = "Action %d has incorrect frame data. Frame line (relative to action): %d.\n";
	static constexpr const char* FR_COUNT_ERR = "Action %d has incorrect information in the header. Should have %d frames, loaded %d.\n";
	static constexpr const char* FR_ORDER_WARN = "WARNING: Frames are not in order. Frame %d created instead of %d!";

	static int charToBind(const char& ch);
public:
	GameLoader(Managers* mgs) : mgs(mgs) {}
	void loadActionData(const char* filePath);
	bool parseHeader(char* line, FILE* file, ActionData* act, 
					int& id, int& seqLen, int& frCount);
	Array<int>* parseSequence(const char* line, int seqLen);
	bool parseFrame(const char* line, FILE* file, ActionFrame& fr, int& frLoaded, int& id);
};