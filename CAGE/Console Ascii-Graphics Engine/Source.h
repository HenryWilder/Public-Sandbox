#pragma once

#undef INCLUDE_VECTOR_INT_2
#undef INCLUDE_VECTOR_FLT_2
#undef INCLUDE_VECTOR_INT_3
#undef INCLUDE_VECTOR_FLT_3
#undef USE_PRINT_CUSTOM
#undef USE_PRINT_PRINTF
#undef USE_PRINT_COUT
#undef INCLUDE_VECTOR_FLT_3
#undef COLOR_T

#include "Config.h"

#ifdef INCLUDE_VECTOR_INT_3
// Existance of vector 3 implies usefulness of vector 2
#define INCLUDE_VECTOR_INT_2
#endif
#ifdef INCLUDE_VECTOR_FLT_3
// Existance of vector 3 implies usefulness of vector 2
#define INCLUDE_VECTOR_FLT_2
#endif

#ifdef INCLUDE_VECTOR_INT_2
#define REQUIRE_VEC_I2(x) x
#else
#define REQUIRE_VEC_I2(x)
#endif
#ifdef INCLUDE_VECTOR_INT_3
#define REQUIRE_VEC_I3(x) x
#else
#define REQUIRE_VEC_I3(x)
#endif
#ifdef INCLUDE_VECTOR_FLT_2
#define REQUIRE_VEC_F2(x) x
#else
#define REQUIRE_VEC_F2(x)
#endif
#ifdef INCLUDE_VECTOR_FLT_3
#define REQUIRE_VEC_F3(x) x
#else
#define REQUIRE_VEC_F3(x)
#endif

#define VEC_TY_CODE_SET(I2,I3,F2,F3) REQUIRE_VEC_I2(I2) REQUIRE_VEC_I3(I3) REQUIRE_VEC_F2(F2) REQUIRE_VEC_F3(F3)

// Custom print has highest priority
#ifdef USE_PRINT_CUSTOM
#undef USE_PRINT_PRINTF
#undef USE_PRINT_COUT
#else
// Printf has second highest priority
#ifdef USE_PRINT_PRINTF
#undef USE_PRINT_COUT
#else
// Cout has third highest priority
#ifdef USE_PRINT_COUT
#else
// Printf is also default
#define USE_PRINT_PRINTF
#endif
#endif
#endif

#ifdef COLOR_T
using Color_t = COLOR_T;
#else
using Color_t = char;
#endif

#ifdef USE_PRINT_CUSTOM
void CUSTOM_PRINT_NAME_PRINT(const Color_t* str);
void CUSTOM_PRINT_NAME_SETCURSOR(size_t x, size_t y);
#define Print CUSTOM_PRINT_NAME_PRINT
#define SetCursor CUSTOM_PRINT_NAME_SETCURSOR
#else
#ifdef USE_PRINT_PRINTF
void Print_Printf(const Color_t* str);
void SetCursor_Printf(size_t x, size_t y);
#define Print Print_Printf
#define SetCursor SetCursor_Printf
#else
#ifdef USE_PRINT_COUT
void Print_Cout(const Color_t* str);
void SetCursor_Cout(size_t x, size_t y);
#define Print Print_Cout
#define SetCursor SetCursor_Cout
#endif
#endif
#endif

float Abs(float x);
signed long long Abs(signed long long x);
float Min(float a, float b);
size_t Min(size_t a, size_t b);
float Max(float a, float b);
size_t Max(size_t a, size_t b);

class ColorRamp
{
	const Color_t* data = nullptr;
	size_t len = 0;

	void Free()
	{
		if (data)
			delete[] data;
	}
	void Replace(const Color_t* arr, size_t size)
	{
		Free();
		len = size;
		Color_t* temp = new Color_t[size];
		for (size_t i = 0; i < size; ++i)
		{
			temp[i] = arr[i];
		}
		data = temp;
	}
public:
	ColorRamp() : data(nullptr), len(0) {}

	template<size_t size>
	ColorRamp(const Color_t(&arr)[size])
	{
		Replace(arr, size);
	}

	template<size_t size>
	ColorRamp& operator=(const Color_t(&arr)[size])
	{
		Replace(arr, size);
	}

	ColorRamp(const Color_t* arr, size_t size)
	{
		Replace(arr, size);
	}

	ColorRamp(const ColorRamp& base)
	{
		Replace(base.data, base.len);
	}

	ColorRamp& operator=(const ColorRamp& base)
	{
		Replace(base.data, base.len);
		return *this;
	}

	~ColorRamp()
	{
		Free();
	}

	void Clear()
	{
		Free();
		data = nullptr;
		len = 0;
	}

	Color_t operator[](size_t i) const
	{
		return data[i];
	}

	size_t Size() const
	{
		return len;
	}
} g_ramp;

void SetColorRamp(const Color_t* ramp, size_t size);
Color_t SampleRamp(float value);

struct Texture
{
	Color_t* tex;
	size_t w, h;
};

size_t EOTex(const Texture* texture); // Out of bounds index

size_t IndexTexture(const Texture* texture, size_t x, size_t y);
Texture CreateTexture(size_t width, size_t height);
void SetTexturePixel(Texture* texture, size_t x, size_t y, Color_t value);
void ClearTexture(Texture* texture, Color_t clearValue);
Color_t GetTexturePixel(const Texture* texture, size_t x, size_t y);

void ApplyTextureRect(Texture* texture, size_t x, size_t y, size_t width, size_t height, Color_t value);
void ApplyTextureLine(Texture* texture, size_t x0, size_t y0, size_t x1, size_t y1, Color_t value);

using VertShaderData = void*; // Can be anything needed

// IVec2-specific structure and functions
#ifdef INCLUDE_VECTOR_INT_2

struct IVec2
{
	int x, y;
};
typedef IVec2(*VertShaderI2)(IVec2);
typedef Color_t(*FragShaderI2)(IVec2, size_t, IVec2*);

size_t IndexTexture(const Texture* texture, IVec2 px);
void SetTexturePixel(Texture* texture, IVec2 px, Color_t value);
Color_t GetTexturePixel(const Texture* texture, IVec2 px);

struct IRect2
{
	int
		x, y,
		w, h;
};

void ApplyTextureRect(Texture* texture, IRect2 rect, Color_t value);

struct ITri2
{
	IVec2 p1, p2, p3;
};

void ApplyTextureTri(Texture* texture, ITri2 tri, Color_t value);

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

void SetTexturePt(Texture* texture, FVec2 pt, Color_t value);
Color_t GetTexturePt(const Texture* texture, FVec2 pt);

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

void Display(const Texture* tex);

#undef Print
#undef SetCursor
