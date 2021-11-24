#include "Source.h"

int main()
{
	SetFrameDimensions(32, 16);
	SetColorRamp(" -+#", sizeof(" -+#") - 1);
	ClearFrame(1);

	DrawPixel(5,5,0);
	DrawRectangle(7,5,7,3,0);

	DisplayDrawnFrame();

	return 0;
}
