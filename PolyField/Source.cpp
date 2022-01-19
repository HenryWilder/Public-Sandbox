#include <windows.h>

int main()
{
    HWND hWnd = GetConsoleWindow();
    HDC hdc = GetDC(hWnd);

    int time = 0;
    
    SelectObject(hdc, GetStockObject(WHITE_PEN));
    SelectObject(hdc, GetStockObject(BLACK_BRUSH));

    POINT pt[30][30];

    while (true)
    {
        POINT cursor;
        GetCursorPos(&cursor);
        ScreenToClient(hWnd, &cursor);

        for (int y = 0; y < 30; ++y)
        {
            for (int x = 0; x < 30; ++x)
            {
                pt[y][x].x = (int)((float)x * 20.0f + (y & 1 ? 10.0f : 0.0f));
                pt[y][x].y = (int)((float)y * 15.0f);

                pt[y][x].x += (((float)rand() / (float)RAND_MAX) - 0.5f) * 7.5f;
                pt[y][x].y += (((float)rand() / (float)RAND_MAX) - 0.5f) * 7.5f;
            }
        }

        for (int y = 1; y < 29; ++y)
        {
            for (int x = 0; x < 29; ++x)
            {
                POINT tri[3];

                tri[0] = pt[y - 1][x + (y & 1)];
                tri[1] = pt[y][x + 1];
                tri[2] = pt[y][x];
                Polygon(hdc, tri, 3);

                tri[0] = pt[y + 1][x + (y & 1)];
                tri[1] = pt[y][x + 1];
                tri[2] = pt[y][x];
                Polygon(hdc, tri, 3);

                SetPixel(hdc, pt[y][x].x, pt[y][x].y, RGB(255,255,255));
            }
        }
    }

    ReleaseDC(hWnd, hdc);

    return 0;
}
