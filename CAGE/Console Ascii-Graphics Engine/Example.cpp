#include "Source.h"

int main()
{
	SetFrameDimensions(32, 16);
	SetColorRamp(" -+#", sizeof(" -+#") - 1);
	ClearFrame(0);

	DisplayDrawnFrame();

	return 0;
}
