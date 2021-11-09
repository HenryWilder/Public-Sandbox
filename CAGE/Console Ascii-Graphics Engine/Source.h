#pragma once

#undef INCLUDE_VECTOR_INT_2
#undef INCLUDE_VECTOR_FLT_2
#undef INCLUDE_VECTOR_INT_3
#undef INCLUDE_VECTOR_FLT_3
#undef COLOR_T

#include "Config.h"

#ifdef INCLUDE_VECTOR_INT_3
#define INCLUDE_VECTOR_INT_2
#endif
#ifdef INCLUDE_VECTOR_FLT_3
#define INCLUDE_VECTOR_FLT_2
#endif

#ifdef COLOR_T
using Color_t = COLOR_T;
#else // Default
using Color_t = char;
#endif

using VertShaderData = void*;

// IVec2-specific structure and functions
#ifdef INCLUDE_VECTOR_INT_2

struct IVec2
{
	int x, y;
};
typedef VertShaderData(*VertShaderI2)(IVec2);
typedef Color_t(*FragShaderI2)(IVec2, VertShaderData);

struct IRect2
{
	int
		x, y,
		w, h;
};

struct ITri2
{
	IVec2 p1, p2, p3;
};

#endif // INCLUDE_VECTOR_INT_2

// FVec2-specific structure and functions
#ifdef INCLUDE_VECTOR_FLT_2

struct FVec2
{
	float x, y;
};
typedef VertShaderData(*VertShaderF2)(FVec2);
typedef Color_t(*FragShaderF2)(FVec2, VertShaderData);

struct FRect2
{
	float
		x, y,
		w, h;
};

struct FTri2
{
	FVec2 p1, p2, p3;
};

#endif // INCLUDE_VECTOR_FLOAT_2

// IVec3-specific structure and functions
#ifdef INCLUDE_VECTOR_INT_3

struct IVec3
{
	int x, y, z;
};
typedef VertShaderData(*VertShaderI3)(IVec3);

struct IRect3
{
	int
		x, y, z,
		w, h, d;
};

struct ITri3
{
	IVec3 p1, p2, p3;
};

#endif // INCLUDE_VECTOR_INT_3

// FVec3-specific structure and functions
#ifdef INCLUDE_VECTOR_FLT_3

struct FVec3
{
	float x, y, z;
};
typedef VertShaderData(*VertShaderF3)(FVec3);

struct FRect3
{
	float
		x, y, z,
		w, h, d;
};

struct FTri3
{
	FVec3 p1, p2, p3;
};

#endif // INCLUDE_VECTOR_FLOAT_3

class CAGE;

class ShaderID
{
	enum class Type
	{
		invalid,

#ifdef INCLUDE_VECTOR_INT_2
#ifdef INCLUDE_VECTOR_INT_3
		vi3,
#endif // INCLUDE_VECTOR_INT_3
		vi2,
		fi2,
#endif // INCLUDE_VECTOR_INT_2

#ifdef INCLUDE_VECTOR_FLT_2
#ifdef INCLUDE_VECTOR_FLT_3
		vf3,
#endif // INCLUDE_VECTOR_FLT_3
		vf2,
		ff2,
#endif // INCLUDE_VECTOR_FLT_2

	} type;
	unsigned int id;

	friend class CAGE;
};

class CAGE
{
	static Color_t* colorRamp; // Array of grayscale values from which to sample
	static unsigned int colorRamp_count; // Number of colors (gray values) in color ramp

	static Color_t* frame; // Frame where pixels are stored to be drawn
	static unsigned int frame_w; // Frame width
	static unsigned int frame_h; // Frame height
	friend void FreeFrame();
	friend void SetFrameDimensions(unsigned int width, unsigned int height);

	// **Shaders**
	// Both static & friend methods used so that the singular "friend class" declaration in the ShaderID type makes the ShaderID accessible to only the methods in the CAGE class
	// Int
#ifdef INCLUDE_VECTOR_INT_2
#ifdef INCLUDE_VECTOR_INT_3

	static VertShaderI3* int3Shaders_vert;
	static unsigned int int3Shaders_vert_count;
	static ShaderID RecognizeShader(VertShaderI3);
	friend ShaderID RecognizeShader(VertShaderI3);

#endif // INCLUDE_VECTOR_INT_3

	static VertShaderI2* int2Shaders_vert;
	static unsigned int int2Shaders_vert_count;
	static ShaderID RecognizeShader(VertShaderI2);
	friend ShaderID RecognizeShader(VertShaderI2);

	static FragShaderI2* int2Shaders_frag;
	static unsigned int int2Shaders_frag_count;
	static ShaderID RecognizeShader(FragShaderI2);
	friend ShaderID RecognizeShader(FragShaderI2);

#endif // INCLUDE_VECTOR_INT_2

	// Float
#ifdef INCLUDE_VECTOR_FLT_2
#ifdef INCLUDE_VECTOR_FLT_3

	static VertShaderF3* flt3Shaders_vert;
	static unsigned int flt3Shaders_vert_count;
	static ShaderID RecognizeShader(VertShaderF3);
	friend ShaderID RecognizeShader(VertShaderF3);

#endif // INCLUDE_VECTOR_INT_3

	static VertShaderF2* flt2Shaders_vert;
	static unsigned int flt2Shaders_vert_count;
	static ShaderID RecognizeShader(VertShaderF2);
	friend ShaderID RecognizeShader(VertShaderF2);

	static FragShaderF2* flt2Shaders_frag;
	static unsigned int flt2Shaders_frag_count;
	static ShaderID RecognizeShader(FragShaderF2);
	friend ShaderID RecognizeShader(FragShaderF2);

#endif // INCLUDE_VECTOR_FLT_2

	static void StartRecordingDrawCalls();
	friend void StartRecordingDrawCalls();
	static void DisplayDrawnFrame();
	friend void DisplayDrawnFrame();
};

ShaderID RecognizeShader(VertShaderI3);
ShaderID RecognizeShader(VertShaderI2);
ShaderID RecognizeShader(FragShaderI2);
ShaderID RecognizeShader(VertShaderF3);
ShaderID RecognizeShader(VertShaderF2);
ShaderID RecognizeShader(FragShaderF2);

void StartRecordingDrawCalls();
void DisplayDrawnFrame();
