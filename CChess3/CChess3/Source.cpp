#include <Windows.h>
#include <windowsx.h>
#include <vector>
#include <fstream>

// Named values
namespace named
{
    constexpr LONG g_spaceWidth = 50;
}
COLORREF g_palette[2][5] = {
    // White
    {
        RGB( 89, 11, 14),
        RGB(119, 14, 18),
        RGB(178, 21, 27),
        RGB(237, 28, 36),
        RGB(241, 73, 80)
    },
    // Black
    {
        RGB( 89, 11, 14),
        RGB(119, 14, 18),
        RGB(178, 21, 27),
        RGB(237, 28, 36),
        RGB(241, 73, 80)
    },
};

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
        constexpr float tick = (1.0f / 60.0f);
        t += tick;
        if (t >= 1.0f)
            t = 0.0f;
    }
    float Random()
    {
        return 6.0f * ((float)(rand() - (RAND_MAX / 2)) / RAND_MAX);
    }
    void Generate()
    {
        start = end;
        control = Random();
        end = Random();
    }
    float Sample() const
    {
        return Lerp(Lerp(start, control, t), Lerp(control, end, t), t);
    }
};
float Animation::t = 0.0f;

struct Triangle
{
    Triangle(std::ifstream& stream, float scale)
    {
        stream >> colorID;
        for (int i = 0; i < 3; ++i)
        {
            stream >> apt[i].x >> apt[i].y;
            apt[i].x = (LONG)((float)apt[i].x * scale);
            apt[i].y = (LONG)((float)apt[i].y * scale);
        }
    }

    int colorID;
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

        float scale;
        file >> scale;
        scale = named::g_spaceWidth / scale;

        unsigned int count;
        file >> count;
        for (unsigned int i = 0; i < count; ++i)
        {
            paths.emplace_back(file, scale);
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
        HBRUSH hBrush = CreateSolidBrush(g_palette[0][tri.colorID]);
        SelectObject(g_hdc, hBrush);
        POINT apt[3];
        for (int i = 0; i < 3; ++i)
        {
            apt[i].x = (tri.apt[i].x + (LONG)x + (LONG)(tri.ptAnim[i][0].Sample() + 0.5f));
            apt[i].y = (tri.apt[i].y + (LONG)y + (LONG)(tri.ptAnim[i][1].Sample() + 0.5f));
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
    VectorGraphic* graphic[64] = {
        nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
          &pawn,  &pawn,  &pawn,  &pawn,  &pawn,  &pawn,  &pawn,  &pawn,
        nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
        nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
        nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
        nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
          &pawn,  &pawn,  &pawn,  &pawn,  &pawn,  &pawn,  &pawn,  &pawn,
        nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
    };

    
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));

    SelectObject(g_hdc, blackBrush);
    Rectangle(g_hdc, 0, 0, 401, 401);

    for (int i = 0; i < 64; ++i)
    {
        int x = (i % 8);
        int y = (i / 8);

        if ((x + y) & 1)
        {
            SelectObject(g_hdc, whiteBrush);
            Rectangle(g_hdc, named::g_spaceWidth * x, named::g_spaceWidth * y, named::g_spaceWidth * (x + 1) + 1, named::g_spaceWidth * (y + 1) + 1);
        }

        if (graphic[i] != nullptr)
            DrawVectorGraphic(*graphic[i], named::g_spaceWidth * x, named::g_spaceWidth * y);
    }


    HBRUSH select = CreateSolidBrush(RGB(0, 255, 0));
    SelectObject(g_hdc, select);

    POINT cursor;

    while (true)
    {
        GetCursorPos(&cursor);
        ScreenToClient(hWnd, &cursor);
        cursor.x /= named::g_spaceWidth;
        cursor.y /= named::g_spaceWidth;

        if (cursor.x >= 0 && cursor.x < 8 &&
            cursor.y >= 0 && cursor.y < 8)
        {
            // Boardspace
            SelectObject(g_hdc, ((cursor.x + cursor.y) & 1 ? whiteBrush : blackBrush));
            int i = cursor.y * 8 + cursor.x;

            // Convert
            cursor.x *= named::g_spaceWidth;
            cursor.y *= named::g_spaceWidth;

            // Screenspace
            Rectangle(g_hdc, cursor.x, cursor.y, cursor.x + named::g_spaceWidth + 1, cursor.y + named::g_spaceWidth + 1);
            if (graphic[i] != nullptr)
                DrawVectorGraphic(*graphic[i], cursor.x, cursor.y);
        }

        Animation::Tick();
        if (Animation::t == 0.0f)
            pawn.Anim_Generate();

        Sleep(16);
    }

    DeleteObject(blackBrush);
    DeleteObject(whiteBrush);
    DeleteObject(select);



    
    

    DeleteObject(hPen);

    GetStockObject(WHITE_BRUSH);
    GetStockObject(DC_PEN);

    ReleaseDC(hWnd, g_hdc);

    return 0;
}