#define _CRT_SECURE_NO_WARNINGS

#include "utils.h"

char* copy_string(const char* str) {
	if (str == nullptr) return nullptr;

	size_t len = strlen(str) + 1;
	char* new_str = new char[len];

	strcpy(new_str, str);

	return new_str;
}

int clamp(int value, int min, int max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

float clamp(float value, float min, float max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}
