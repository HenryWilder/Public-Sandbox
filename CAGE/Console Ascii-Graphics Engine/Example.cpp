#include "Source.h"

int main()
{
	SetFrameDimensions(32, 16);
	SetColorRamp("0123456789", 10);
	StartRecordingDrawCalls();
	ClearFrame(3.0f / 10.0f);
	DisplayDrawnFrame();

	return 0;
}
