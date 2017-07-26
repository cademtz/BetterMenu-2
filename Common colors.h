#pragma once
#include "CDrawManager.h"

class Colors
{
public:
	DWORD black = COLORCODE(0, 0, 0, 255);
	DWORD white = COLORCODE(255, 255, 255, 255);
	DWORD light = COLORCODE(200, 200, 200, 255);
	DWORD menuBack = COLORCODE(20, 20, 20, 200);
	DWORD dark = COLORCODE(30, 30, 30, 255);
	DWORD bluTeam = COLORCODE(0, 128, 255, 255);
	DWORD redTeam = COLORCODE(186, 52, 53, 255);
	DWORD gray = COLORCODE(90, 90, 90, 255);
	DWORD lightGray = COLORCODE(130, 130, 130, 255);
	DWORD lightBlue = COLORCODE(78, 128, 182, 255);
	DWORD dullOrange = COLORCODE(155, 72, 36, 255);
	DWORD orange = COLORCODE(199, 107, 16, 255);
	DWORD red = COLORCODE(237, 28, 36, 255);
	DWORD dullRed = COLORCODE(182, 90, 78, 255);
	DWORD green = COLORCODE(37, 197, 53, 255);
};
extern Colors colors;