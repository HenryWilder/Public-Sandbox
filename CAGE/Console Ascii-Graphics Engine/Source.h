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
#define INCLUDE_VECTOR_INT_2
#endif
#ifdef INCLUDE_VECTOR_FLT_3
#define INCLUDE_VECTOR_FLT_2
#endif

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

namespace Templates
{
	// A dynamic array which grows but never shrinks and cannot have elements erased, allowing for integer IDs to never be invalidated.
	// Elements can be replaced.
	template<typename Ty>
	class Growable
	{
		Ty* data;
		size_t count;
		size_t space;

	public:
		Growable()
		{
			data = nullptr;
			count = 0;
			space = 1;
		}
		~Growable()
		{
			Free();
		}
		size_t Size() const
		{
			return count;
		}
		Ty& operator[](size_t i)
		{
			return data[i];
		}
		const Ty& operator[](size_t i) const
		{
			return data[i];
		}
		// How many NEW elements to reserve space for (not total)
		void Reserve(size_t extensionSize)
		{
			if (data)
			{
				Ty* newLocation = new Ty[space += extensionSize];
				for (size_t i = 0; i < count; ++i)
				{
					newLocation[i] = data[i];
				}
				delete[] data;
				data = newLocation;
			}
			else
				data = new Ty[space += extensionSize];
		}
		size_t Push(const Ty& element)
		{
			if (count == space)
				Reserve(space);

			data[count] = element;
			return count++;
		}
		void Free()
		{
			if (data)
			{
				delete[] data;
				data = nullptr;
				count = space = 0;
			}
		}
		void Replace(size_t i, const Ty& replacement)
		{
			data[i] = replacement;
		}
	};

	// An array that can have its entire data replaced at once. It cannot be resized any other way.
	// Allows for a swappable "palette" of sorts.
	template<typename Ty>
	class Series
	{
		const Ty* data = nullptr;
		size_t len = 0;

		void Free()
		{
			if (data)
				delete[] data;
		}
		void Replace(const Ty* arr, size_t size)
		{
			Free();
			len = size;
			Ty* temp = new Ty[size];
			for (size_t i = 0; i < size; ++i)
			{
				temp[i] = arr[i];
			}
			data = temp;
		}
	public:
		Series() : data(nullptr), len(0) {}

		template<size_t size>
		Series(const Ty(&arr)[size])
		{
			Replace(arr, size);
		}

		template<size_t size>
		Series& operator=(const Ty(&arr)[size])
		{
			Replace(arr, size);
		}

		Series(const Ty* arr, size_t size)
		{
			Replace(arr, size);
		}

		Series(const Series& base)
		{
			Replace(base.data, base.len);
		}

		Series& operator=(const Series& base)
		{
			Replace(base.data, base.len);
			return *this;
		}

		~Series()
		{
			Free();
		}

		void Clear()
		{
			Free();
			data = nullptr;
			len = 0;
		}

		Ty operator[](size_t i) const
		{
			return data[i];
		}

		size_t Size() const
		{
			return len;
		}
	};

	// A dynamic array which can only be accessed at the back, which can be grown or shrank without copying prior elements
	template<typename Ty>
	class LinkedStack
	{
		class Element
		{
			Ty value;
			Element* prev;
			Element* next;
		};

		Element* tail;

	public:
		LinkedStack()
		{
			tail = nullptr;
		}
		~LinkedStack()
		{
			if (tail)
			{
				while (tail->prev)
				{
					tail = tail->prev;
					delete tail->next;
				}
				delete tail;
			}
		}

		void Pop()
		{
			if (tail)
			{
				if (tail->prev)
				{
					tail = tail->prev;
					delete tail->next;
					tail->next = nullptr;
				}
				else
				{
					delete tail;
					tail = nullptr;
				}
			}
		}
		void Push(const Ty& element)
		{
			if (tail)
			{
				tail->next = new Element;
				tail->next->prev = tail;
				tail = tail->next;
				tail->value = element;
			}
			else
			{
				tail = new Element;
				tail->prev = tail->next = nullptr;
				tail->value = element;
			}
		}

		Ty& Top()
		{
			return tail->value;
		}
	};
}

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

using VertShaderData = void*;

// IVec2-specific structure and functions
#ifdef INCLUDE_VECTOR_INT_2

struct IVec2
{
	int x, y;
};
typedef VertShaderData(*VertShaderI2)(IVec2);
typedef Color_t(*FragShaderI2)(IVec2, VertShaderData);

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

class CAGE;

class ShaderID
{
	enum class Type
	{
		invalid,

#ifdef INCLUDE_VECTOR_INT_2
#ifdef INCLUDE_VECTOR_INT_3
		i3,
#endif // INCLUDE_VECTOR_INT_3
		i2,
#endif // INCLUDE_VECTOR_INT_2

#ifdef INCLUDE_VECTOR_FLT_2
#ifdef INCLUDE_VECTOR_FLT_3
		f3,
#endif // INCLUDE_VECTOR_FLT_3
		f2,
#endif // INCLUDE_VECTOR_FLT_2

	} type;

	size_t vsID, fsID;

	friend class CAGE;

#ifdef INCLUDE_VECTOR_INT_2
#ifdef INCLUDE_VECTOR_INT_3
	friend ShaderID RecognizeShader(VertShaderI3, FragShaderI2);
#endif // INCLUDE_VECTOR_INT_3
	friend ShaderID RecognizeShader(VertShaderI2, FragShaderI2);
#endif // INCLUDE_VECTOR_INT_2
#ifdef INCLUDE_VECTOR_FLT_2
#ifdef INCLUDE_VECTOR_FLT_3
	friend ShaderID RecognizeShader(VertShaderF3, FragShaderF2);
#endif // INCLUDE_VECTOR_FLT_3
	friend ShaderID RecognizeShader(VertShaderF2, FragShaderF2);
#endif // INCLUDE_VECTOR_FLT_2
};

class CAGE
{
	static Templates::Series<Color_t> colorRamp; // Array of grayscale values from which to sample

	friend void SetColorRamp(const Color_t* ramp, size_t size);
	friend Color_t SampleRamp(float value);

	static Texture frame;
	friend void SetFrameDimensions(size_t width, size_t height);
	friend void ClearFrame(float value);

	friend void DrawPixel(size_t x, size_t y, float value);
	friend void DrawRectangle(size_t x, size_t y, size_t width, size_t height, float value);

	// **Shaders**

	// Int
#ifdef INCLUDE_VECTOR_INT_2

	static Templates::Growable<FragShaderI2> ShadersI2_frag;

#ifdef INCLUDE_VECTOR_INT_3

	static Templates::Growable<VertShaderI3> ShadersI3_vert;
	friend ShaderID RecognizeShader(VertShaderI3 vertexShader, FragShaderI2 fragShader);

#endif // INCLUDE_VECTOR_INT_3

	static Templates::Growable<VertShaderI2> ShadersI2_vert;
	friend ShaderID RecognizeShader(VertShaderI2 vertexShader, FragShaderI2 fragShader);

#endif // INCLUDE_VECTOR_INT_2

	// Float
#ifdef INCLUDE_VECTOR_FLT_2

	static Templates::Growable<FragShaderF2> ShadersF2_frag;

#ifdef INCLUDE_VECTOR_FLT_3

	static Templates::Growable<VertShaderF3> ShadersF3_vert;
	friend ShaderID RecognizeShader(VertShaderF3 vertexShader, FragShaderF2 fragShader);

#endif // INCLUDE_VECTOR_FLT_3

	static Templates::Growable<VertShaderF2> ShadersF2_vert;
	friend ShaderID RecognizeShader(VertShaderF2 vertexShader, FragShaderF2 fragShader);

#endif // INCLUDE_VECTOR_FLT_2

	// Remembers what functions are called so that only intersecting pixels are drawn.
	// I'm not sure whether this will even be helpful, since storing & recalling all the overlaps could be slower
	// than simply drawing overtop of the same pixels in cases where the RAM is cheaper than the CPU. (99.999999% of cases)
	// Still, it might be useful in fringe cases.
	friend void RecordDrawCallsForCulling();
	
	// Displays the frame using the print function
	friend void DisplayDrawnFrame();
};

void SetColorRamp(const Color_t* ramp, size_t size);
Color_t SampleRamp(float value);
void SetFrameDimensions(size_t width, size_t height);
void ClearFrame(float value);

void DrawPixel(size_t x, size_t y, float value);
void DrawRectangle(size_t x, size_t y, size_t width, size_t height, float value);

#ifdef INCLUDE_VECTOR_INT_3
ShaderID RecognizeShader(VertShaderI3 vertexShader, FragShaderI2 fragShader);
#endif
#ifdef INCLUDE_VECTOR_INT_2
ShaderID RecognizeShader(VertShaderI2 vertexShader, FragShaderI2 fragShader);
#endif
#ifdef INCLUDE_VECTOR_FLT_3
ShaderID RecognizeShader(VertShaderF3 vertexShader, FragShaderF2 fragShader);
#endif
#ifdef INCLUDE_VECTOR_FLT_2
ShaderID RecognizeShader(VertShaderF2 vertexShader, FragShaderF2 fragShader);
#endif

void RecordDrawCallsForCulling();
void DisplayDrawnFrame();

#undef Print
#undef SetCursor
