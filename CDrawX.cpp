#include "CDrawX.h"
#include <vector>

Colors colors;
CTextures Textures;
CDrawManagerEx gDrawEx;

void CTextures::init()
{
	unsigned char const darkGray[4] = { 50, 50, 50, 255 }; // r,g,b,a
	Textures.darkGray = gInts.Surface->CreateNewTextureID(true); // need to make a texture with procedural true
	gInts.Surface->DrawSetTextureRGBA(Textures.darkGray, darkGray, 1, 1);

	unsigned char const lightGray[4] = { 80, 80, 80, 255 };
	Textures.lightGray = gInts.Surface->CreateNewTextureID(true);
	gInts.Surface->DrawSetTextureRGBA(Textures.lightGray, lightGray, 1, 1);

	unsigned char const light[4] = { 200, 200, 200, 255 };
	Textures.light = gInts.Surface->CreateNewTextureID(true);
	gInts.Surface->DrawSetTextureRGBA(Textures.light, light, 1, 1);
}

DWORD CDrawManagerEx::addColor(DWORD color1, DWORD color2)
{
	unsigned char c1[4], c2[4];
	c1[0] = RED(color1), c1[1] = BLUE(color1), c1[2] = GREEN(color1), c1[3] = ALPHA(color1);
	c2[0] = RED(color2), c2[1] = BLUE(color2), c2[2] = GREEN(color2), c2[3] = ALPHA(color2);

	for (int i = 0; i < 4; i++)
		if (c1[i] + c2[i] > 255)
			c1[i] = 255;
		else
			c1[i] += c2[i];

	return COLORCODE(c1[0], c1[1], c1[2], c1[3]);
}

void CDrawManagerEx::DrawLine(int x, int y, int x1, int y1, DWORD dwColor)
{
	gInts.Surface->DrawSetColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	gInts.Surface->DrawLine(x, y, x1, y1);
}

void CDrawManagerEx::PolyLine(int *x, int *y, int count, DWORD dwColor)
{
	gInts.Surface->DrawSetColor(RED(dwColor), BLUE(dwColor), GREEN(dwColor), ALPHA(dwColor));
	gInts.Surface->DrawPolyLine(x, y, count);
}

void CDrawManagerEx::DrawPolygon(int count, Vertex_t* verts, int texture)
{
	//static int Texture = gInts.Surface->CreateNewTextureID(true); // need to make a texture with procedural true
	//unsigned char const buffer[4] = { RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor) }; // r,g,b,a

	//gInts.Surface->DrawSetTextureRGBA(Texture, buffer, 1, 1); //Texture, char array of texture, width, height
	gInts.Surface->DrawSetColor(255, 255, 255, 255); // keep this full color and opacity use the RGBA @top to set values.
	gInts.Surface->DrawSetTexture(texture); // bind texture

	gInts.Surface->DrawTexturedPolygon(count, verts, true);
}

void CDrawManagerEx::RainbowRect(int x, int y, int w, int h, bool Vertical, int saturation, int value, int alpha)
{
	if (saturation > 255)
		saturation = 255;
	if (value > 255)
		value = 255;
	if (alpha > 255)
		alpha = 255;

	float s = (float)saturation / 255, v = (float)value / 255;

	if (Vertical)
	{
		for (int i = 0; i < h; i++)
			gDraw.DrawRect(x, y, w, 1, FromHSB((float) i / h, s, v, alpha));
	}
	else
	{
		for (int i = 0; i < w; i++)
			gDraw.DrawRect(x + i, y, 1, h, FromHSB((float) i / w, s, v, alpha));
	}
}

void CDrawManagerEx::GradientRect(int x, int y, int w, int h, bool VerticalGradient, DWORD startColor, DWORD endColor)
{
	int rStep = RED(endColor) - RED(startColor);
	int gStep = GREEN(endColor) - GREEN(startColor);
	int bStep = BLUE(endColor) - BLUE(startColor);
	int aStep = ALPHA(endColor) - ALPHA(startColor);

	if (VerticalGradient)
	{
		rStep /= h, gStep /= h, bStep /= h, aStep /= h;
		DWORD color = startColor;

		for (int i = 0; i < h; i++)
		{
			if (i == h - 1)
				color = endColor;
			gDraw.DrawRect(x, y + i, w, 1, color);
			color = COLORCODE(RED(color) + rStep, GREEN(color) + gStep, BLUE(color) + bStep, ALPHA(color) + aStep);
		}
	}
	else
	{
		rStep /= w, gStep /= w, bStep /= w, aStep /= w;
		DWORD color = startColor;

		for (int i = 0; i < w; i++)
		{
			if (i == w - 1)
				color = endColor;
			gDraw.DrawRect(x + i, y, 1, h, color);
			color = COLORCODE(RED(color) + rStep, GREEN(color) + gStep, BLUE(color) + bStep, ALPHA(color) + aStep);
		}
	}
}

void CDrawManagerEx::DrawString( int x, int y, DWORD dwColor, unsigned long font, const char *pszText, ...)
{
	if (pszText == NULL)
		return;

	va_list va_alist;
	char szBuffer[1024] = { '\0' };
	wchar_t szString[1024] = { '\0' };

	va_start(va_alist, pszText);
	vsprintf_s(szBuffer, pszText, va_alist);
	va_end(va_alist);

	wsprintfW(szString, L"%S", szBuffer);

	gInts.Surface->DrawSetTextPos(x, y);
	gInts.Surface->DrawSetTextFont(font);
	gInts.Surface->DrawSetTextColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	gInts.Surface->DrawPrintText(szString, wcslen(szString));
}

void CDrawManagerEx::DrawCircle(int x, int y, int radius, int segments, DWORD dwColor)
{
	gInts.Surface->DrawSetColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	gInts.Surface->DrawOutlinedCircle(x, y, radius, segments);
}

void CDrawManagerEx::OutlineCircle(int x, int y, int radius, int segments, DWORD dwColor)
{
	gInts.Surface->DrawSetColor(RED(dwColor), GREEN(dwColor), BLUE(dwColor), ALPHA(dwColor));
	gInts.Surface->DrawOutlinedCircle(x, y, radius, segments);
}

void CDrawManagerEx::RoundRect(int x, int y, int w, int h, int radius, int texture)
{
	Vertex_t round[64];

	for (int i = 0; i < 4; i++)
	{
		int _x = x + ((i < 2) ? (w - radius) : radius);
		int _y = y + ((i % 3) ? (h - radius) : radius);

		float a = 90.f * i;

		for (int j = 0; j < 16; j++)
		{
			float _a = DEG2RAD(a + j * 6.f);

			round[(i * 16) + j] = Vertex_t(Vector2D(_x + radius * sin(_a), _y - radius * cos(_a)));
		}
	}

	DrawPolygon(64, round, texture);
}

void CDrawManagerEx::OutlineRoundRect(int x, int y, int w, int h, int radius, DWORD dwColor)
{
	int xpos[64], ypos[64];

	for (int i = 0; i < 4; i++)
	{
		int _x = x + ((i < 2) ? (w - radius) : radius);
		int _y = y + ((i % 3) ? (h - radius) : radius);

		float a = 90.f * i;

		for (int j = 0; j < 16; j++)
		{
			float _a = DEG2RAD(a + j * 6.f);

			xpos[(i * 16) + j] = _x + radius * sin(_a), ypos[(i * 16) + j] = _y - radius * cos(_a);
		}
	}

	PolyLine(xpos, ypos, 64, dwColor);
}

void CDrawManagerEx::CornerRect(int x, int y, int w, int h, int xRadius, int yRadius, DWORD dwColor)
{
	gDrawEx.DrawLine(x, y, x + xRadius, y, dwColor); // Upper left
	gDrawEx.DrawLine(x, y, x, y + yRadius, dwColor);

	gDrawEx.DrawLine(x + w, y, x + w - xRadius, y, dwColor); // Upper right
	gDrawEx.DrawLine(x + w, y, x + w, y + yRadius, dwColor);

	gDrawEx.DrawLine(x, y + h, x + xRadius, y + h, dwColor); // Lower left
	gDrawEx.DrawLine(x, y + h, x, y + h - yRadius, dwColor);

	gDrawEx.DrawLine(x + w, y + h, x + w - xRadius, y + h, dwColor); // Lower right
	gDrawEx.DrawLine(x + w, y + h, x + w, y + h - yRadius, dwColor);
}

DWORD CDrawManagerEx::FromHSB(float hue, float saturation, float brightness, int alpha)
{
	float h = hue == 1.0f ? 0 : hue * 6.0f;
	float f = h - (int)h;
	float p = brightness * (1.0f - saturation);
	float q = brightness * (1.0f - saturation * f);
	float t = brightness * (1.0f - (saturation * (1.0f - f)));

	if (h < 1)
	{
		return COLORCODE(
			(unsigned char)(brightness * 255),
			(unsigned char)(t * 255),
			(unsigned char)(p * 255),
			alpha
		);
	}
	else if (h < 2)
	{
		return COLORCODE(
			(unsigned char)(q * 255),
			(unsigned char)(brightness * 255),
			(unsigned char)(p * 255),
			alpha
		);
	}
	else if (h < 3)
	{
		return COLORCODE(
			(unsigned char)(p * 255),
			(unsigned char)(brightness * 255),
			(unsigned char)(t * 255),
			alpha
		);
	}
	else if (h < 4)
	{
		return COLORCODE(
			(unsigned char)(p * 255),
			(unsigned char)(q * 255),
			(unsigned char)(brightness * 255),
			alpha
		);
	}
	else if (h < 5)
	{
		return COLORCODE(
			(unsigned char)(t * 255),
			(unsigned char)(p * 255),
			(unsigned char)(brightness * 255),
			alpha
		);
	}
	else
	{
		return COLORCODE(
			(unsigned char)(brightness * 255),
			(unsigned char)(p * 255),
			(unsigned char)(q * 255),
			alpha
		);
	}
}