#include "Fonts.h"
#include "CDrawX.h"

Fonts fonts;

const wchar_t* Fonts::to_wchar(const char* str)
{
	if (str == NULL)
		return NULL;

	va_list va_alist;
	char szBuffer[1024] = { '\0' };
	wchar_t szString[1024] = { '\0' };

	va_start(va_alist, str);
	vsprintf_s(szBuffer, str, va_alist);
	va_end(va_alist);

	wsprintfW(szString, L"%S", szBuffer);

	return szString;
}

string Fonts::floatStr(float input)
{
	string text = to_string(input);
	int end = 1;

	for (int i = text.length() - 1; i != 0; i--)
	{
		if (text[i] == '.')
		{
			end = i;
			break;
		}
		else if (text[i] != '0')
		{
			end = i + 1;
			break;
		}
	}

	string output = "";
	for (int i = 0; i < end; i++)
		output += text[i];

	return output;
}

void Fonts::init()
{
	itemFont = gInts.Surface->CreateFont();
	gInts.Surface->SetFontGlyphSet(
		fonts.itemFont,
		"Tahoma", 15, 1800, 0, 0,
		FONTFLAG_ANTIALIAS);

	titleFont = gInts.Surface->CreateFont();
	gInts.Surface->SetFontGlyphSet(
		fonts.titleFont,
		"Impact", 26, 1800, 0, 0,
		FONTFLAG_ANTIALIAS);

	outlinedFont = gInts.Surface->CreateFont();
	gInts.Surface->SetFontGlyphSet(
		fonts.outlinedFont,
		"Consolas", 14, 500, 0, 0,
		FONTFLAG_OUTLINE + FONTFLAG_ANTIALIAS);

	smallFont = gInts.Surface->CreateFont();
	gInts.Surface->SetFontGlyphSet(
		fonts.smallFont,
		"Arial", 14, 2000, 0, 0,
		FONTFLAG_ANTIALIAS);

	tf2 = gInts.Surface->CreateFont();
	gInts.Surface->SetFontGlyphSet(
		fonts.tf2,
		"TF2 Build", 13, 500, 0, 0,
		FONTFLAG_OUTLINE);

	lbox = gInts.Surface->CreateFont();
	gInts.Surface->SetFontGlyphSet(
		fonts.lbox,
		"Impact", 30, 500, 0, 0,
		FONTFLAG_OUTLINE + FONTFLAG_ANTIALIAS);

	cambria = gInts.Surface->CreateFont();
	gInts.Surface->SetFontGlyphSet(
		fonts.cambria,
		"Cambria", 48, 1000, 0, 0,
		FONTFLAG_ANTIALIAS);

	impact = gInts.Surface->CreateFont();
	gInts.Surface->SetFontGlyphSet(
		fonts.impact,
		"Impact", 16, 400, 0, 0,
		FONTFLAG_OUTLINE + FONTFLAG_ANTIALIAS
	);

	fancy = gInts.Surface->CreateFont();
	gInts.Surface->SetFontGlyphSet(
		fonts.fancy,
		"Gabriola", 24, 600, 0, 0,
		FONTFLAG_OUTLINE + FONTFLAG_ANTIALIAS
	);
}