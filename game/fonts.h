#pragma once

#include "config.h"

inline constexpr Font COMBO_FONT = {
	RES::CH_64,
	64,
	1.0f,
	.6f,
	Colors::white,
	{Colors::black, 4},
	0
};

inline constexpr Font COMBO_MUL_FONT = {
	RES::CH_64,
	64,
	2.0f,
	.6f,
	Colors::yellow,
	{Colors::black, 4},
	0
};

inline constexpr Font HITS_LIT_FONT = {
	RES::CH_64,
	64,
	.5f,
	.5f,
	Colors::white,
	{Colors::black, 2},
	0
};

inline constexpr Font TITLE_FONT = {
	RES::CH_32,
	32,
	1.5f,
	.7f,
	ColorRGBA::white(),
	{ { 0x1E, 0x1E, 0x1E, 0xFF }, 6 },
	6
};

inline constexpr Font H2_FONT = {
	RES::CH_32,
	32,
	1.0f,
	.7f,
	ColorRGBA::white(),
	{ { 0x1E, 0x1E, 0x1E, 0xFF }, 4 },
	6
};

inline constexpr Font BUTTON_FONT = {
	RES::CH_32,
	32,
	1.25f,
	.75f,
	ColorRGBA::white(),
	Outline(),
	6
};

inline constexpr Font INPUT_FONT = {
	RES::CH_16,
	16,
	1.75f,
	1.0f,
	{ 0x1E, 0x1E, 0x1E, 0xFF },
	Outline(),
	-2
};

inline constexpr Font INFO_FONT = {
		RES::CH_16,
		16,
		.75f,
		1.0f,
		ColorRGBA::black()
};

inline constexpr Font DEBUG_ACT_FONT = { 
	RES::CH_16, 
	16, 
	3.0, 
	1.0, 
	Colors::white, 
	{ Colors::black, 2 }, 
	0 
};