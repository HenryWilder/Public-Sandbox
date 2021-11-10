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

Templates::Series<Color_t> CAGE::colorRamp;
Texture CAGE::frame;

void SetColorRamp(const Color_t* ramp, size_t size)
{
	CAGE::colorRamp = Templates::Series<Color_t>(ramp, size);
}

void SetFrameDimensions(size_t width, size_t height)
{
	if (CAGE::frame.tex)
		delete[] CAGE::frame.tex;

	CAGE::frame = CreateTexture(width, height);
}
void ClearFrame(float value)
{
	ClearTexture(&CAGE::frame, CAGE::colorRamp[(int)(value * (float)CAGE::colorRamp.Size())]);
}

#ifdef INCLUDE_VECTOR_INT_2

Templates::Growable<FragShaderI2> CAGE::ShadersI2_frag;

#ifdef INCLUDE_VECTOR_INT_3

Templates::Growable<VertShaderI3> CAGE::ShadersI3_vert;

ShaderID RecognizeShader(VertShaderI3 vertexShader, FragShaderI2 fragShader)
{
	ShaderID id;
	id.type = ShaderID::Type::i3;
	id.vsID = CAGE::ShadersI3_vert.Push(vertexShader);
	id.fsID = CAGE::ShadersI2_frag.Push(fragShader);
	return id;
}

#endif // INCLUDE_VECTOR_INT_3

Templates::Growable<VertShaderI2> CAGE::ShadersI2_vert;

ShaderID RecognizeShader(VertShaderI2 vertexShader, FragShaderI2 fragShader)
{
	ShaderID id;
	id.type = ShaderID::Type::i2;
	id.vsID = CAGE::ShadersI2_vert.Push(vertexShader);
	id.fsID = CAGE::ShadersI2_frag.Push(fragShader);
	return id;
}

#endif // INCLUDE_VECTOR_INT_2

#ifdef INCLUDE_VECTOR_FLT_2

Templates::Growable<FragShaderF2> CAGE::ShadersF2_frag;

#ifdef INCLUDE_VECTOR_FLT_3

Templates::Growable<VertShaderF3> CAGE::ShadersF3_vert;

ShaderID RecognizeShader(VertShaderF3 vertexShader, FragShaderF2 fragShader)
{
	ShaderID id;
	id.type = ShaderID::Type::f3;
	id.vsID = CAGE::ShadersF3_vert.Push(vertexShader);
	id.fsID = CAGE::ShadersF2_frag.Push(fragShader);
	return id;
}

#endif // INCLUDE_VECTOR_FLT_3

Templates::Growable<VertShaderF2> CAGE::ShadersF2_vert;

ShaderID RecognizeShader(VertShaderF2 vertexShader, FragShaderF2 fragShader)
{
	ShaderID id;
	id.type = ShaderID::Type::f2;
	id.vsID = CAGE::ShadersF2_vert.Push(vertexShader);
	id.fsID = CAGE::ShadersF2_frag.Push(fragShader);
	return id;
}

#endif // INCLUDE_VECTOR_FLT_2

void StartRecordingDrawCalls()
{
	// Todo
}

void DisplayDrawnFrame()
{
	SetCursor(0, 0);
	Print(CAGE::frame.tex);
}

void SetTexturePixel(Texture* texture, size_t x, size_t y, Color_t value)
{
	if (x < texture->w && y < texture->h)
		texture->tex[y * (texture->w + 1) + x] = value;
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
	return texture->tex[y * (texture->w + 1) + x];
}

void SetTexturePixel(Texture* texture, IVec2 px, Color_t value)
{
	SetTexturePixel(texture, px.x, px.y, value);
}

void ApplyTextureTri(Texture* texture, ITri2 tri, Color_t value)
{
	// Todo
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
