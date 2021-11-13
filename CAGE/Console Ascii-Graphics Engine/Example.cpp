#include "Source.h"

int main()
{
	SetFrameDimensions(32, 16);
	SetColorRamp(" #", 10);
	ClearFrame(0);

	DisplayDrawnFrame();

	return 0;
}
