#pragma once
#include "SDK.h"
#include <string>

class Fonts
{
public:
	unsigned long titleFont;
	unsigned long itemFont;
	unsigned long tf2;
	unsigned long lbox;
	unsigned long outlinedFont;
	unsigned long smallFont;
	unsigned long cambria;
	unsigned long impact;
	unsigned long fancy;

	void init();
	const wchar_t* to_wchar(const char* str);
	string floatStr(float input);
};
extern Fonts fonts;