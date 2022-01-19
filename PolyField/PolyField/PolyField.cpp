#include <windows.h>
#include <algorithm>

float Lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

struct Vector2
{
    float x, y;
};
struct AnimatedPoint
{
    AnimatedPoint() : pt{ 0,0 }, cp{ { 0,0 }, { 0,0 }, { RandomFlt(), RandomFlt() } }, agitation(0.0f) {}

    static float t;
    POINT pt;
    Vector2 cp[3];
    float agitation;

    float RandomFlt()
    {
        return (((float)rand() / (float)RAND_MAX) - 0.5f) * 2.0f;
    }
    void Generate()
    {
        cp[0] = cp[2];
        cp[1] = { RandomFlt(), RandomFlt() };
        cp[2] = { RandomFlt(), RandomFlt() };
    }
    // True if need to generate
    static bool Tick()
    {
        constexpr float tick = 2.0f / 125.0f;
        t += tick;
        if (t > 1.0f)
        {
            t -= 1.0f;
            return true;
        }
        else
            return false;
    }
    POINT GetPos()
    {
        return {
            pt.x + (LONG)(0.5f + Lerp(5.0f, 20.0f, agitation) * Lerp(Lerp(cp[0].x, cp[1].x, t), Lerp(cp[1].x, cp[2].x, t), t)),
            pt.y + (LONG)(0.5f + Lerp(5.0f, 20.0f, agitation) * Lerp(Lerp(cp[0].y, cp[1].y, t), Lerp(cp[1].y, cp[2].y, t), t))
        };
    }
};
float AnimatedPoint::t = 0.0f;

int main()
{
    HWND hWnd = GetConsoleWindow();
    HDC hdc = GetDC(hWnd);

    int time = 0;

    SelectObject(hdc, GetStockObject(WHITE_PEN));
    SelectObject(hdc, GetStockObject(BLACK_BRUSH));

    AnimatedPoint pt[30][30];
    for (int y = 0; y < 30; ++y)
    {
        for (int x = 0; x < 30; ++x)
        {
            pt[y][x].pt.x = (int)((float)x * 20.0f + (y & 1 ? 10.0f : 0.0f)) - 30;
            pt[y][x].pt.y = (int)((float)y * 15.0f) - 30;
            pt[y][x].Generate();
        }
    }

    while (true)
    {
        if (AnimatedPoint::Tick())
        {
            for (int y = 0; y < 30; ++y)
            {
                for (int x = 0; x < 30; ++x)
                {
                    pt[y][x].Generate();
                }
            }
        }

        POINT cursor;
        GetCursorPos(&cursor);
        ScreenToClient(hWnd, &cursor);

        for (int y = 0; y < 30; ++y)
        {
            for (int x = 0; x < 30; ++x)
            {
                float a = cursor.x - pt[y][x].GetPos().x;
                float b = cursor.y - pt[y][x].GetPos().y;
                float dist = sqrtf(a * a + b * b);
                pt[y][x].agitation = (dist > 200.0f ? 0.0f : (200.0f - dist) / 200.0f);
            }
        }

        for (int y = 1; y < 29; ++y)
        {
            for (int x = 0; x < 29; ++x)
            {
                POINT tri[3];

                tri[0] = pt[y - 1][x + (y & 1)].GetPos();
                tri[1] = pt[y][x + 1].GetPos();
                tri[2] = pt[y][x].GetPos();
                Polygon(hdc, tri, 3);

                tri[0] = pt[y + 1][x + (y & 1)].GetPos();
                tri[1] = pt[y][x + 1].GetPos();
                tri[2] = pt[y][x].GetPos();
                Polygon(hdc, tri, 3);

                SetPixel(hdc, pt[y][x].GetPos().x, pt[y][x].GetPos().y, RGB(255, 255, 255));
            }
        }

        Sleep(16);
    }

    ReleaseDC(hWnd, hdc);

    return 0;
}
