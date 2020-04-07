#pragma once
#include <cstdint>

// using DB16 - DawnBringer's 16 Col Palette v1.0
// http://pixeljoint.com/forum/forum_posts.asp?TID=12795
enum DB16_COLOUR
{
	FG_Black			= 0x0000,
	FG_Dark_Magenta		= 0x0001,
	FG_Dark_Blue		= 0x0002,
	FG_Gray				= 0x0003,
	FG_Light_Brown		= 0x0004,
	FG_Grass_Green		= 0x0005,
	FG_Red				= 0x0006,
	FG_Ditry_Gray		= 0x0007,
	FG_Blue				= 0x0008,
	FG_Light_Green		= 0x0009,
	FG_Metal			= 0x000A,
	FG_Acid_Green		= 0x000B,
	FG_Skin				= 0x000C,
	FG_Sky				= 0x000D,
	FG_Honey			= 0x000E,
	FG_Moon_White		= 0x000F,

	BG_Black			= 0x0000,
	BG_Dark_Magenta		= 0x0010,
	BG_Dark_Blue		= 0x0020,
	BG_Gray				= 0x0030,
	BG_Light_Brown		= 0x0040,
	BG_Grass_Green		= 0x0050,
	BG_Red				= 0x0060,
	BG_Ditry_Gray		= 0x0070,
	BG_Blue				= 0x0080,
	BG_Light_Green		= 0x0090,
	BG_Metal			= 0x00A0,
	BG_Acid_Green		= 0x00B0,
	BG_Skin				= 0x00C0,
	BG_Sky				= 0x00D0,
	BG_Honey			= 0x00E0,
	BG_Moon_White		= 0x00F0,
};

// Default color scheme:
enum class DEF_COLOUR
{
	DFG_BLACK			= 0x0000,
	DFG_DARK_BLUE		= 0x0001,
	DFG_DARK_GREEN		= 0x0002,
	DFG_DARK_CYAN		= 0x0003,
	DFG_DARK_RED		= 0x0004,
	DFG_DARK_MAGENTA	= 0x0005,
	DFG_DARK_YELLOW		= 0x0006,
	DFG_GREY			= 0x0007,
	DFG_DARK_GREY		= 0x0008,
	DFG_BLUE			= 0x0009,
	DFG_GREEN			= 0x000A,
	DFG_CYAN			= 0x000B,
	DFG_RED				= 0x000C,
	DFG_MAGENTA			= 0x000D,
	DFG_YELLOW			= 0x000E,
	DFG_WHITE			= 0x000F,

	DBG_BLACK			= 0x0000,
	DBG_DARK_BLUE		= 0x0010,
	DBG_DARK_GREEN		= 0x0020,
	DBG_DARK_CYAN		= 0x0030,
	DBG_DARK_RED		= 0x0040,
	DBG_DARK_MAGENTA	= 0x0050,
	DBG_DARK_YELLOW		= 0x0060,
	DBG_GREY			= 0x0070,
	DBG_DARK_GREY		= 0x0080,
	DBG_BLUE			= 0x0090,
	DBG_GREEN			= 0x00A0,
	DBG_CYAN			= 0x00B0,
	DBG_RED				= 0x00C0,
	DBG_MAGENTA			= 0x00D0,
	DBG_YELLOW			= 0x00E0,
	DBG_WHITE			= 0x00F0,
};
enum class PIXEL_TYPE
{
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
	PIXEL_NONE = ' '
};

class ConsoleColor : public CHAR_INFO
{
public:
	ConsoleColor(PIXEL_TYPE pixel, DEF_COLOUR color) 
		:CHAR_INFO{ (wchar_t)pixel, (uint16_t)color}
	{

	}
};