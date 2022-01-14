#include <Windows.h>
#include <windowsx.h>
#include <vector>
#include <fstream>

HDC g_hdc;

float Lerp(float a, float b, float t)
{
    return a + t * (b - a);
}
 
struct Animation
{
    Animation()
    {
        end = 0.0f;
        Generate();
    }

    float start, control, end;
    static float t;

    static void Tick()
    {
        constexpr float tick = (1.0f / 120.0f);
        t += tick;
        if (t >= 1.0f)
            t = 0.0f;
    }
    void Generate()
    {
        start = end;
        control = 6.0f * ((float)(rand() - (RAND_MAX / 2)) / RAND_MAX);
        end = 6.0f * ((float)(rand() - (RAND_MAX / 2)) / RAND_MAX);
    }
    float Sample() const
    {
        return Lerp(Lerp(start, control, t), Lerp(control, end, t), t);
    }
};
float Animation::t = 0.0f;

struct Triangle
{
    Triangle(std::ifstream& stream)
    {
        {
            int r, g, b;
            stream >> r >> g >> b;
            color = RGB(r, g, b);
        }
        for (int i = 0; i < 3; ++i)
        {
            stream >> apt[i].x >> apt[i].y;
        }
    }
    Triangle(
        int r, int g, int b,
        LONG x0, LONG y0,
        LONG x1, LONG y1,
        LONG x2, LONG y2)
    {
        color = RGB(r,g,b);
        apt[0].x = x0;
        apt[0].y = y0;
        apt[1].x = x1;
        apt[1].y = y1;
        apt[2].x = x2;
        apt[2].y = y2;
    }

    COLORREF color;
    POINT apt[3];
    Animation ptAnim[3][2];

    void Anim_Generate()
    {
        for (unsigned int i = 0; i < 3; ++i)
        {
            for (unsigned int j = 0; j < 2; ++j)
            {
                ptAnim[i][j].Generate();
            }
        }
    }
};
struct VectorGraphic
{
    VectorGraphic(const char* filename)
    {
        std::ifstream file(filename);

        unsigned int count;
        file >> count;
        for (unsigned int i = 0; i < count; ++i)
        {
            paths.emplace_back(file);
        }

        file.close();
    }

    std::vector<Triangle> paths;

    void Anim_Generate()
    {
        for (Triangle& tri : paths)
        {
            tri.Anim_Generate();
        }
    }
};

void DrawVectorGraphic(const VectorGraphic& vg, int x, int y)
{
    for (const Triangle& tri : vg.paths)
    {
        HBRUSH hBrush = CreateSolidBrush(tri.color);
        SelectObject(g_hdc, hBrush);
        POINT apt[3];
        for (int i = 0; i < 3; ++i)
        {
            apt[i].x = tri.apt[i].x + (LONG)x + (LONG)(tri.ptAnim[i][0].Sample() + 0.5f);
            apt[i].y = tri.apt[i].y + (LONG)y + (LONG)(tri.ptAnim[i][1].Sample() + 0.5f);
        }
        Polygon(g_hdc, apt, 3);
        DeleteObject(hBrush);
    }
}

int main()
{
    HWND hWnd = GetConsoleWindow();
    g_hdc = GetDC(hWnd);

    HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
    SelectObject(g_hdc, hPen);

    VectorGraphic pawn("pawn");

    for (int i = 0; i < 800; ++i)
    {
        {
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(g_hdc, hBrush);
            Rectangle(g_hdc, 0, 0, 200, 100);
            DeleteObject(hBrush);
        }
        DrawVectorGraphic(pawn, 000, 0);
        DrawVectorGraphic(pawn, 100, 0);

        Sleep(8);

        Animation::Tick();
        if (Animation::t == 0.0f)
            pawn.Anim_Generate();
    }

    DeleteObject(hPen);

    GetStockObject(WHITE_BRUSH);
    GetStockObject(DC_PEN);

    ReleaseDC(hWnd, g_hdc);

    return 0;
}
