#define _CRT_SECURE_NO_WARNINGS

#include "utils.h"

char* copy_string(const char* a_Str) {
	if (a_Str == nullptr) return nullptr;

	size_t len = strlen(a_Str) + 1;
	char* new_str = new char[len];

	strcpy(new_str, a_Str);

	return new_str;
}

int clamp(int a_Val, int a_Min, int a_Max)
{
	if (a_Val < a_Min) return a_Min;
	if (a_Val > a_Max) return a_Max;
	return a_Val;
}

float clamp(float a_Val, float a_Min, float a_Max)
{
	if (a_Val < a_Min) return a_Min;
	if (a_Val > a_Max) return a_Max;
	return a_Val;
}
