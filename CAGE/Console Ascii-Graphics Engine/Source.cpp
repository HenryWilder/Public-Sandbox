#include "Source.h"

#ifdef USE_PRINT_PRINTF

#include <stdio.h>

void Print_Printf(const Color_t* str)
{
	printf(str);
}

void SetCursor_Printf(size_t x, size_t y)
{
	printf("\x1b[%i;%if", y, x);
}

#define Print Print_Printf
#define SetCursor SetCursor_Printf

#else

#ifdef USE_PRINT_COUT

#include <iostream>

void Print_Cout(const Color_t* str)
{
	std::cout << str;
}

void SetCursor_Cout(size_t x, size_t y)
{
	std::cout << "\x1b[" << y << ';' << x << 'f';
}

#define Print Print_Cout
#define SetCursor SetCursor_Cout

#else

#define Print CUSTOM_PRINT_NAME_PRINT
#define SetCursor CUSTOM_PRINT_NAME_SETCURSOR

#endif

#endif

float Abs(float x)
{
	return (x < 0.0f ? -x : x);
}
signed long long Abs(signed long long x)
{
	return (x < 0 ? -x : x);
}
float Min(float a, float b)
{
	return (b < a ? b : a);
}
size_t Min(size_t a, size_t b)
{
	return (b < a ? b : a);
}
float Max(float a, float b)
{
	return (b > a ? b : a);
}
size_t Max(size_t a, size_t b)
{
	return (b > a ? b : a);
}

ColorRamp g_ramp;

void SetColorRamp(const Color_t* ramp, size_t size)
{
	g_ramp = ColorRamp(ramp, size);
}

Color_t SampleRamp(float value)
{
	if (value > 1.0f) value = 1.0f;
	else if (value < 0.0f) value = 0.0f;
	return g_ramp[(int)(value * (float)(g_ramp.Size() - 1))];
}

size_t EOTex(const Texture* texture)
{
	if (!texture)
		return 0;
	else
		return (texture->w + 1) * texture->h;
}

size_t IndexTexture(const Texture* texture, size_t x, size_t y)
{
	if (!texture || x >= texture->w || y >= texture->h)
		return EOTex(texture);
	else
		return (texture->w + 1) * y + x;
}

Texture CreateTexture(size_t width, size_t height)
{
	Texture t;
	t.tex = new Color_t[(width + 1) * height];
	for (size_t i = 0; i < (height); ++i)
	{
		t.tex[(width + 1) * i - 1] = '\n';
	}
	t.tex[(width + 1) * height - 1] = 0;
	t.w = width;
	t.h = height;
	return t;
}

void SetTexturePixel(Texture* texture, size_t x, size_t y, Color_t value)
{
	size_t index = IndexTexture(texture, x, y);
	if (index != EOTex(texture))
		texture->tex[index] = value;
}

void ClearTexture(Texture* texture, Color_t clearValue)
{
	for (size_t y = 0; y < texture->h; ++y)
	{
		for (size_t x = 0; x < texture->w; ++x)
		{
			texture->tex[y * (texture->w + 1) + x] = clearValue;
		}
	}
}

Color_t GetTexturePixel(const Texture* texture, size_t x, size_t y)
{
	size_t index = IndexTexture(texture, x, y);
	if (index != EOTex(texture))
		return texture->tex[index];
	else
		return (Color_t)0;
}

void ApplyTextureRect(Texture* texture, size_t x, size_t y, size_t width, size_t height, Color_t value)
{
	for (size_t _y = y; _y < (y + height) && _y < texture->h; ++_y)
	{
		size_t lineStart = IndexTexture(texture, x, _y);
		for (size_t _x = 0; _x < width && (x + _x) < texture->w; ++_x)
		{
			texture->tex[lineStart + _x] = value;
		}
	}
}

void ApplyTextureLine(Texture* texture, size_t x0, size_t y0, size_t x1, size_t y1, Color_t value)
{
	if (y0 == y1 && x0 == x1)
	{
		SetTexturePixel(texture, x0, y0, value);
	}
	else if (y0 == y1)
	{
		size_t start = IndexTexture(texture, 0, y0);
		for (size_t x = Max(x0, 0); x <= Min(x1, texture->w - 1); ++x)
		{
			texture->tex[start + x] = value;
		}
	}
	else if (x0 == x1)
	{
		size_t start = IndexTexture(texture, 0, y0);
		size_t offs = texture->w + 1;
		for (size_t y = Max(y0, 0); y <= Min(y1, texture->h - 1); ++y)
		{
			texture->tex[start + y * offs] = value;
		}
	}
	else
	{
		size_t xLil, xBig, yLil, yBig;

		if (x0 > x1)
		{
			xLil = x1;
			xBig = x0;
		}
		else
		{
			xLil = x0;
			xBig = x1;
		}

		if (y0 > y1)
		{
			yLil = y1;
			yBig = y0;
		}
		else
		{
			yLil = y0;
			yBig = y1;
		}

		size_t dx = xBig - xLil;
		size_t dy = yBig - yLil;
		size_t D = 2 * dy - dx;
		size_t y = yLil;

		for (size_t x = xLil; x <= xBig; ++x)
		{
			SetTexturePixel(texture, x, y, value);
			if (D > 0)
			{
				++y;
				D -= 2 * dx;
			}
		}
	}
}

#ifdef INCLUDE_VECTOR_INT_2

size_t IndexTexture(const Texture* texture, IVec2 px)
{
	return IndexTexture(texture, px.x, px.y);
}

void SetTexturePixel(Texture* texture, IVec2 px, Color_t value)
{
	SetTexturePixel(texture, px.x, px.y, value);
}

Color_t GetTexturePixel(const Texture* texture, IVec2 px)
{
	return GetTexturePixel(texture, px.x, px.y);
}


void ApplyTextureRect(Texture* texture, IRect2 rect, Color_t value)
{
	for (size_t y = rect.y; y < rect.y + rect.h && y < texture->h; ++y)
	{
		for (size_t x = rect.x; x < rect.x + rect.w && x < texture->w; ++x)
		{
			texture->tex[y * (texture->w + 1) + x] = value;
		}
	}
}

void ApplyTextureTri(Texture* texture, ITri2 tri, Color_t value)
{
	// Todo
}

#endif

#ifdef INCLUDE_VECTOR_FLT_2

void SetTexturePt(Texture* texture, FVec2 pt, Color_t value)
{
	SetTexturePixel(texture, (size_t)(pt.x + 0.5f), (size_t)(pt.y + 0.5f), value);
}

Color_t GetTexturePt(const Texture* texture, FVec2 pt)
{
	return GetTexturePixel(texture, (size_t)(pt.x + 0.5f), (size_t)(pt.y + 0.5f));
}

#endif

void Display(const Texture* tex)
{
	SetCursor(0,0);
	Print(tex->tex);
}
