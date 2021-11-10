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

namespace Templates
{
	size_t Method_Double(size_t currentSize);
	size_t Method_Increment(size_t currentSize);

	// A dynamic array which grows but never shrinks and cannot have elements erased, allowing for integer IDs to never be invalidated.
	// Elements can be replaced.
	template<typename Ty, size_t(*SpaceToAllocate)(size_t) = Method_Increment> class Growable
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
				Reserve(SpaceToAllocate(space));

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
	template<typename Ty, bool b_UseConstReference = (sizeof(Ty)>sizeof(Ty*))> class Array {};

	// An array that can have its entire data replaced at once. It cannot be resized any other way.
	// Allows for a swappable "palette" of sorts.
	template<typename Ty> class Array<Ty,false>
	{
		const Ty* data;
		bool stack;
		size_t count;

		void Free()
		{
			if (data && !stack)
				delete[] data;
		}

	public:
		Array()
		{
			data = nullptr;
			stack = false;
			count = 0;
		}
		~Array()
		{
			Free();
		}

		size_t Size() const
		{
			return count;
		}

		// Copies each element from the input array, allowing the passed in array to be freed without issue.
		// Good for when you want to keep using the passed array for other things.
		void SetSafe_Heap(const Ty* dna, size_t size)
		{
			Free();
			stack = false;
			Ty* replacement = new Ty[size];
			for (size_t i = 0; i < count = size; ++i)
			{
				replacement[i] = dna[i];
			}
			data = replacement;
		}

		// Uses the exact pointer which was passed in, posing the risk of unannounced memory deletion (or breaking if the memory is deleted externally!),
		// but skipping the copy step and handling memory freeing automatically.
		// Good for arrays allocated for exclusively this purpose.
		void SetFast_Heap(const Ty* arr, size_t size)
		{
			Free();
			stack = false;
			data = arr;
			count = size;
		}

		// Copies each element from the input array, allowing the passed in array to be popped without issue.
		// Good for one-off arrays you don't care about and just want to type in easily.
		void SetSafe_Stack(const Ty(&& arr)[size])
		{
			Free();
			stack = true;
			Ty* replacement = new Ty[size];
			for (size_t i = 0; i < count = size; ++i)
			{
				replacement[i] = dna[i];
			}
			data = replacement;
		}

		// Uses the exact pointer which was passed in, posing the risk of unannounced memory deletion (or breaking if the array is popped externally!),
		// but skipping the copy step and handling memory freeing automatically.
		// Good for arrays pushed to the stack (at the highest scope this class will be used in!) for exclusively this purpose.
		template<size_t size> void SetFast_Stack(const Ty(&arr)[size])
		{
			Free();
			stack = true;
			data = arr;
			count = size;
		}

		Ty operator[](size_t i)
		{
			return data[i];
		}
	};

	// An array that can have its entire data replaced at once. It cannot be resized any other way.
	// Allows for a swappable "palette" of sorts.
	template<typename Ty> class Array<Ty,true>
	{
		const Ty* data;
		bool stack;
		size_t count;

		void Free()
		{
			if (data && !stack)
				delete[] data;
		}

	public:
		Array()
		{
			data = nullptr;
			stack = false;
			count = 0;
		}
		~Array()
		{
			Free();
		}

		size_t Size() const
		{
			return count;
		}

		// Copies each element from the input array, allowing the passed in array to be freed without issue.
		// Good for when you want to keep using the passed array for other things.
		void SetSafe_Heap(const Ty* dna, size_t size)
		{
			Free();
			stack = false;
			Ty* replacement = new Ty[size];
			for (size_t i = 0; i < count = size; ++i)
			{
				replacement[i] = dna[i];
			}
			data = replacement;
		}

		// Uses the exact pointer which was passed in, posing the risk of unannounced memory deletion (or breaking if the memory is deleted externally!),
		// but skipping the copy step and handling memory freeing automatically.
		// Good for arrays allocated for exclusively this purpose.
		void SetFast_Heap(const Ty* arr, size_t size)
		{
			Free();
			stack = false;
			data = arr;
			count = size;
		}

		// Copies each element from the input array, allowing the passed in array to be popped without issue.
		// Good for one-off arrays you don't care about and just want to type in easily.
		void SetSafe_Stack(const Ty (&&arr)[size])
		{
			Free();
			stack = true;
			Ty* replacement = new Ty[size];
			for (size_t i = 0; i < count = size; ++i)
			{
				replacement[i] = dna[i];
			}
			data = replacement;
		}

		// Uses the exact pointer which was passed in, posing the risk of unannounced memory deletion (or breaking if the array is popped externally!),
		// but skipping the copy step and handling memory freeing automatically.
		// Good for arrays pushed to the stack (at the highest scope this class will be used in!) for exclusively this purpose.
		template<size_t size> void SetFast_Stack(const Ty (&arr)[size])
		{
			Free();
			stack = true;
			data = arr;
			count = size;
		}

		const Ty& operator[](size_t i)
		{
			return data[i];
		}
	};

	// A dynamic array which can only be accessed at the back, which can be grown or shrank without copying prior elements
	template<typename Ty> class LinkedStack
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
					tail->next = nullptr
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

#ifdef COLOR_T
using Color_t = COLOR_T;
#else // Default
using Color_t = char;
#endif

struct Texture
{
	Color_t* tex;
	size_t w, h;
};

Texture CreateTexture(size_t width, size_t height);
void SetTexturePixel(Texture* texture, size_t x, size_t y, Color_t value);
void ClearTexture(Texture* texture, Color_t clearValue);
Color_t GetTexturePixel(const Texture* texture, size_t x, size_t y);

using VertShaderData = void*;

// IVec2-specific structure and functions
#ifdef INCLUDE_VECTOR_INT_2

struct IVec2
{
	int x, y;
};
typedef VertShaderData(*VertShaderI2)(IVec2);
typedef Color_t(*FragShaderI2)(IVec2, VertShaderData);

void SetTexturePixel(Texture* texture, IVec2 px, Color_t value);
Color_t GetTexturePixel(const Texture* texture, IVec2 px);

struct IRect2
{
	int
		x, y,
		w, h;
};

void ApplyTextureRect(Texture* texture, IRect2 tri, Color_t value);

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

void SetTexturePt(Texture* texture, FVec2 px, Color_t value);
Color_t GetTexturePt(const Texture* texture, FVec2 px);

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

#ifdef INCLUDE_VECTOR_INT_3
		vi3fi2,
#endif // INCLUDE_VECTOR_INT_3

#ifdef INCLUDE_VECTOR_INT_2
		vi2fi2,
#endif // INCLUDE_VECTOR_INT_2

#ifdef INCLUDE_VECTOR_FLT_3
		vf3fi2,
#endif // INCLUDE_VECTOR_FLT_3

#ifdef INCLUDE_VECTOR_FLT_2
		vf2fi2,
#endif // INCLUDE_VECTOR_FLT_2

	} type;

	unsigned int vsID, fsID;

	friend class CAGE;
};

class CAGE
{
	static Templates::Array<Color_t> colorRamp; // Array of grayscale values from which to sample

	static Texture frame;

	friend void SetFrameDimensions(size_t width, size_t height);

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

void SetFrameDimensions(size_t width, size_t height);

ShaderID RecognizeShader(VertShaderI3);
ShaderID RecognizeShader(VertShaderI2);
ShaderID RecognizeShader(FragShaderI2);
ShaderID RecognizeShader(VertShaderF3);
ShaderID RecognizeShader(VertShaderF2);
ShaderID RecognizeShader(FragShaderF2);

void StartRecordingDrawCalls();
void DisplayDrawnFrame();
