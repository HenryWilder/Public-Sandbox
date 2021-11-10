#include "Source.h"

size_t Growable_Double(size_t currentSize)
{
	return currentSize > 0 ? currentSize : 1;
}
size_t Growable_Increment(size_t currentSize)
{
	return 1;
}

void SetFrameDimensions(size_t width, size_t height)
{
	if (CAGE::frame.tex)
		delete[] CAGE::frame.tex;


}

#ifdef INCLUDE_VECTOR_INT_2
#ifdef INCLUDE_VECTOR_INT_3

ShaderID CAGE::RecognizeShader(VertShaderI3 shader)
{
	int3Shaders_vert_count++;
	VertShaderI3* replacement = new VertShaderI3[int3Shaders_vert_count];
	for (unsigned int i = 0; i < int3Shaders_vert_count - 1; ++i)
	{
		replacement[i] = int3Shaders_vert[i];
	}
	delete[] int3Shaders_vert;
	int3Shaders_vert = replacement;
}
void SetFrameDimensions(size_t width, size_t height)
{
	CAGE::frame = Texture(width, height);
}
ShaderID RecognizeShader(VertShaderI3 shader)
{
	return CAGE::RecognizeShader(shader);
}

#endif // INCLUDE_VECTOR_INT_3

ShaderID CAGE::RecognizeShader(VertShaderI2 shader)
{
	int2Shaders_vert_count++;
	VertShaderI2* replacement = new VertShaderI2[int2Shaders_vert_count];
	for (unsigned int i = 0; i < int2Shaders_vert_count - 1; ++i)
	{
		replacement[i] = int2Shaders_vert[i];
	}
	delete[] int2Shaders_vert;
	int2Shaders_vert = replacement;
}
ShaderID RecognizeShader(VertShaderI2 shader)
{
	return CAGE::RecognizeShader(shader);
}

ShaderID CAGE::RecognizeShader(FragShaderI2 shader)
{
	int2Shaders_frag_count++;
	FragShaderI2* replacement = new FragShaderI2[int2Shaders_frag_count];
	for (unsigned int i = 0; i < int2Shaders_frag_count - 1; ++i)
	{
		replacement[i] = int2Shaders_frag[i];
	}
	delete[] int2Shaders_frag;
	int2Shaders_frag = replacement;
}
ShaderID RecognizeShader(FragShaderI2 shader)
{
	return CAGE::RecognizeShader(shader);
}

#endif // INCLUDE_VECTOR_INT_2

#ifdef INCLUDE_VECTOR_FLT_2
#ifdef INCLUDE_VECTOR_FLT_3

ShaderID CAGE::RecognizeShader(VertShaderF3 shader)
{
	int3Shaders_vert_count++;
	VertShaderF3* replacement = new VertShaderF3[flt3Shaders_vert_count];
	for (unsigned int i = 0; i < flt3Shaders_vert_count - 1; ++i)
	{
		replacement[i] = flt3Shaders_vert[i];
	}
	delete[] flt3Shaders_vert;
	flt3Shaders_vert = replacement;
}
ShaderID RecognizeShader(VertShaderF3 shader)
{
	return CAGE::RecognizeShader(shader);
}

#endif // INCLUDE_VECTOR_FLT_3

ShaderID CAGE::RecognizeShader(VertShaderF2 shader)
{
	flt2Shaders_vert_count++;
	VertShaderF2* replacement = new VertShaderF2[flt2Shaders_vert_count];
	for (unsigned int i = 0; i < flt2Shaders_vert_count - 1; ++i)
	{
		replacement[i] = flt2Shaders_vert[i];
	}
	delete[] flt2Shaders_vert;
	flt2Shaders_vert = replacement;
}
ShaderID RecognizeShader(VertShaderF2 shader)
{
	return CAGE::RecognizeShader(shader);
}

ShaderID CAGE::RecognizeShader(FragShaderF2 shader)
{
	flt2Shaders_frag_count++;
	FragShaderF2* replacement = new FragShaderF2[flt2Shaders_frag_count];
	for (unsigned int i = 0; i < flt2Shaders_frag_count - 1; ++i)
	{
		replacement[i] = flt2Shaders_frag[i];
	}
	delete[] flt2Shaders_frag;
	flt2Shaders_frag = replacement;
}
ShaderID RecognizeShader(FragShaderF2 shader)
{
	return CAGE::RecognizeShader(shader);
}

#endif // INCLUDE_VECTOR_FLT_2

void CAGE::StartRecordingDrawCalls()
{

}
void StartRecordingDrawCalls()
{
	CAGE::StartRecordingDrawCalls();
}

void CAGE::DisplayDrawnFrame()
{

}
void DisplayDrawnFrame()
{
	CAGE::DisplayDrawnFrame();
}

Texture CreateTexture(size_t width, size_t height)
{
	return Texture();
}

void SetTexturePixel(Texture* texture, size_t x, size_t y, Color_t value)
{
	texture->tex[y * texture->w + x] = value;
}

void ClearTexture(Texture* texture, Color_t clearValue)
{
	for (size_t i = 0; i < texture->h * texture->w; ++i)
	{
		texture->tex[i] = clearValue;
	}
}

Color_t GetTexturePixel(const Texture* texture, size_t x, size_t y)
{
	return texture->tex[y * texture->w + x];
}

void SetTexturePixel(Texture* texture, IVec2 px, Color_t value)
{
	SetTexturePixel(texture, px.x, px.y, value);
}

void ApplyTextureTri(Texture* texture, ITri2 tri, Color_t value)
{

}

Texture CreateTexture(size_t width, size_t height)
{
	Texture t;
	t.tex = new Color_t[width * height];
	t.w = width;
	t.h = height;
	return t;
}
