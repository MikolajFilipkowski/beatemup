#pragma once

#include <cstdio>
#include "../core/core.h"
#include "config.h"

class GameLoader {
private:
	Managers* m_Mgs{};

	static constexpr int MAX_LINE_LEN = 512;

	static constexpr const char* HEADER_FORMAT = "[%d] %d %d %d %x %d %d %d %d %x";
	static constexpr int HEADER_VARS = 10;
	static constexpr const char* HEADER_ERR = "Action file has incorrect header data.\n";

	static constexpr const char* NAME_ERR = "Action name too long! Max 64 characters.\n";

	static constexpr const char* FR_FORMAT = "< %d > %f %f %f %f %f | %f %f %f %f | %f %f %f %f";
	static constexpr int FR_VARS = 14;
	static constexpr const char* FR_ERR = "Action %d has incorrect frame data. Frame line (relative to action): %d.\n";
	static constexpr const char* FR_COUNT_ERR = "Action %d has incorrect information in the header. Should have %d frames, loaded %d.\n";
	static constexpr const char* FR_ORDER_WARN = "WARNING: Frames are not in order. Frame %d created instead of %d!\n";

	static int charToBind(const char& a_Char);
public:
	GameLoader() {}
	void init(Managers* a_Managers);
	void loadActionData(const char* a_FilePath);
	bool parseHeader(char* a_Line, FILE* a_File, ActionData* a_Act, 
					int& a_Id, int& a_SeqLen, int& a_FrCount);
	void parseName(char* a_Line, ActionData* a_Act);
	Array<int>* parseSequence(const char* line, int seqLen);
	bool parseFrame(const char* a_Line, FILE* a_File, ActionFrame& a_Fr, int& a_FrLoaded, int& a_Id);
};