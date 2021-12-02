#include "Source.h"

int main()
{
	Texture frame = CreateTexture(32, 16);
	SetColorRamp(" -+#", 4);

	SetTexturePixel();

	Display(&frame);

	return 0;
}
