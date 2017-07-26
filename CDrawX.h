#pragma once
#include "CDrawManager.h"
#include "CTextures.h"
#include "Common colors.h"

using namespace std;

#define FONTFLAG_NONE 0x000
#define FONTFLAG_ITALIC 0x001
#define FONTFLAG_UNDERLINE 0x002
#define FONTFLAG_STRIKEOUT 0x004
#define FONTFLAG_SYMBOL 0x008
#define FONTFLAG_ANTIALIAS 0x010
#define FONTFLAG_GAUSSIANBLUR 0x020
#define FONTFLAG_ROTARY 0x040
#define FONTFLAG_DROPSHADOW 0x080
#define FONTFLAG_ADDITIVE 0x100
#define FONTFLAG_OUTLINE 0x200
#define FONTFLAG_CUSTOM 0x400

class CDrawManagerEx
{
public:
	void DrawString( int x, int y, DWORD dwColor, unsigned long font, const char *pszText, ... );
	void DrawCircle(int x, int y, int radius, int segments, DWORD dwColor);
	void OutlineCircle(int x, int y, int radius, int segments, DWORD dwColor);
	void DrawLine(int x, int y, int x1, int y1, DWORD dwColor);
	void DrawPolygon(int count, Vertex_t* verts, int texture);
	void PolyLine(int *x, int *y, int count, DWORD dwColor);
	DWORD FromHSB(float hue, float saturation, float brightness, int alpha);
	// If VerticalGradiant is false, then it's a horizontal gradient
	// The startColor is the color at the top or left side of the rectangle
	void GradientRect(int x, int y, int w, int h, bool VerticalGradient, DWORD startColor, DWORD endColor);
	// If Vertical is true, the rainbow will start from the top
	void RainbowRect(int x, int y, int w, int h, bool Vertical, int saturation, int value, int alpha);
	void RoundRect(int x, int y, int w, int h, int radius, int texture);
	void OutlineRoundRect(int x, int y, int w, int h, int radius, DWORD dwColor);
	void CornerRect(int x, int y, int w, int h, int xRadius, int yRadius, DWORD dwColor);
	void CornerRect(int x, int y, int w, int h, int radius, DWORD dwColor)
	{
		CornerRect(x, y, w, h, radius, radius, dwColor);
	}
	DWORD addColor(DWORD color1, DWORD color2);
};
extern CDrawManagerEx gDrawEx;

/*class TextureHolder
{
	protected:
	uint32_t m_iTexture;
	uint32_t m_iW, m_iH;
	DWORD  m_bgColor;
	bool   m_bValid;
	
	ISurface* m_pSurface;
	public:
	typedef shared_ptr<TextureHolder> Ptr;
	TextureHolder()
	: m_pSurface(gInts.Surface), m_iH(0), m_iW(0), m_bgColor COLORCODE(255, 255, 255, 255), m_bValid(false)
	{ };
	
	TextureHolder(const unsigned char* pRawRGBAData, uint32_t W, uint32_t H)
	: m_pSurface(gInts.Surface), m_iH(H), m_iW(W), m_bgColor COLORCODE(255, 255, 255, 255), m_bValid(false)
	{
		m_iTexture = m_pSurface->CreateNewTextureID(true);
		if (!m_iTexture)
		return;
		m_pSurface->DrawSetTextureRGBA(m_iTexture, pRawRGBAData, W, H);
		m_bValid = true;
	};
	
	bool IsValid() const
	{
		return m_bValid;
	};
	
	int GetTextureId() const
	{
		return m_iTexture;
	};
	
	bool Draw(int x, int y, float scale = 1.0)
	{
		if (!m_pSurface->IsTextureIDValid(m_iTexture))
		return false;
		m_pSurface->DrawSetColor(RED(m_bgColor), GREEN(m_bgColor), BLUE(m_bgColor), ALPHA(m_bgColor));
		m_pSurface->DrawSetTexture(m_iTexture);
		m_pSurface->DrawTexturedRect(x, y, x + m_iW * scale, y + m_iH * scale);
		return true;
	};
};*/