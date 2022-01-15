#include <Windows.h>
#include <windowsx.h>
#include <vector>
#include <fstream>

// Named values
namespace named
{
    constexpr LONG g_spaceWidth = 100;
}
COLORREF g_palette[2][6] = {
    // White
    {
        RGB(59, 7, 9),
        RGB(119, 14, 18),
        RGB(178, 21, 27),
        RGB(237, 28, 36),
        RGB(241, 73, 80),
        RGB(245, 245, 245)
    },
    // Black
    {
        RGB(9, 7, 59),
        RGB(18, 14, 119),
        RGB(27, 21, 178),
        RGB(36, 28, 237),
        RGB(80, 73, 241),
        RGB(30, 30, 30)
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

    static float agitation;

    static void Tick()
    {
        t += (1.0f / Lerp(60.0f, 20.0f, agitation));
        if (t >= 1.0f)
            t = 0.0f;
    }
    static float Random()
    {
        return Lerp(5.0f * (100.0f / (float)named::g_spaceWidth), 15.0f * (100.0f / (float)named::g_spaceWidth), agitation) * ((float)(rand() - (RAND_MAX / 2)) / RAND_MAX);
    }
    void Generate()
    {
        start = end;
        control = Random();
        end = Random();
    }
    // Sets the start control point and t to 0
    void FromZero()
    {
        start = 0;
        t = 0.0f;
    }
    float Sample() const
    {
        return Lerp(Lerp(start, control, t), Lerp(control, end, t), t);
    }
};
float Animation::t = 0.0f;
float Animation::agitation = 0.0f;

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
    void Anim_Zero()
    {
        for (unsigned int i = 0; i < 3; ++i)
        {
            for (unsigned int j = 0; j < 2; ++j)
            {
                ptAnim[i][j].FromZero();
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
    void Anim_Zero()
    {
        for (Triangle& tri : paths)
        {
            tri.Anim_Zero();
        }
    }
};

void DrawVectorGraphicAnimated(const VectorGraphic& vg, int x, int y, int team)
{
    for (const Triangle& tri : vg.paths)
    {
        HBRUSH hBrush = CreateSolidBrush(g_palette[team & 1][tri.colorID]);
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
void DrawVectorGraphic(const VectorGraphic& vg, int x, int y, int team)
{
    for (const Triangle& tri : vg.paths)
    {
        HBRUSH hBrush = CreateSolidBrush(g_palette[team & 1][tri.colorID]);
        SelectObject(g_hdc, hBrush);
        POINT apt[3];
        for (int i = 0; i < 3; ++i)
        {
            apt[i].x = (tri.apt[i].x + (LONG)x);
            apt[i].y = (tri.apt[i].y + (LONG)y);
        }
        Polygon(g_hdc, apt, 3);
        DeleteObject(hBrush);
    }
}

VectorGraphic g_graphics[6] = {
    VectorGraphic("pawn.cc3asset"),
    VectorGraphic("rook.cc3asset"),
    VectorGraphic("knight.cc3asset"),
    VectorGraphic("bishop.cc3asset"),
    VectorGraphic("queen.cc3asset"),
    VectorGraphic("king.cc3asset")
};

enum class UnitType
{
    pawn = 0,
    rook = 1,
    knight = 2,
    bishop = 3,
    queen = 4,
    king = 5,
};
struct Unit
{
    Unit(UnitType _type, int _team) : team(_team), type(_type) {}

    int team;
    UnitType type;
    
    VectorGraphic* GetGraphic() const
    {
        return g_graphics + (int)type;
    }
};

Unit g_units[12] = {
        Unit(UnitType::pawn, 0),
        Unit(UnitType::rook, 0),
        Unit(UnitType::knight, 0),
        Unit(UnitType::bishop, 0),
        Unit(UnitType::queen, 0),
        Unit(UnitType::king, 0),

        Unit(UnitType::pawn, 1),
        Unit(UnitType::rook, 1),
        Unit(UnitType::knight, 1),
        Unit(UnitType::bishop, 1),
        Unit(UnitType::queen, 1),
        Unit(UnitType::king, 1)
};

Unit* g_board[64] = {
    g_units + 1,  g_units + 2, g_units + 3, g_units + 4, g_units + 5,  g_units + 3, g_units + 2, g_units + 1,
    g_units + 0,  g_units + 0, g_units + 0, g_units + 0, g_units + 0,  g_units + 0, g_units + 0, g_units + 0,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
    g_units + 6,  g_units + 6, g_units + 6, g_units + 6, g_units + 6,  g_units + 6, g_units + 6, g_units + 6,
    g_units + 7,  g_units + 8, g_units + 9, g_units + 10, g_units + 11,  g_units + 9, g_units + 8, g_units + 7,
};

LONG BoardToIndex(POINT space)
{
    return space.y * 8 + space.x;
}
LONG BoardToIndex(int space_x, int space_y)
{
    return space_y * 8 + space_x;
}
POINT IndexToBoard(LONG index)
{
    return POINT{ index % (LONG)8, index / (LONG)8 };
}
void BoardToScreen(POINT* space)
{
    space->x *= named::g_spaceWidth;
    space->y *= named::g_spaceWidth;
}
void BoardToScreen(int* space_x, int* space_y)
{
    *space_x *= named::g_spaceWidth;
    *space_y *= named::g_spaceWidth;
}
void ScreenToBoard(POINT* pt)
{
    pt->x /= named::g_spaceWidth;
    pt->y /= named::g_spaceWidth;
}
void ScreenToBoard(int* pt_x, int* pt_y)
{
    *pt_x /= named::g_spaceWidth;
    *pt_y /= named::g_spaceWidth;
}

bool IsSpaceOnBoard(int space_x, int space_y)
{
    return
        space_x >= 0 && space_x < 8 &&
        space_y >= 0 && space_y < 8;
}
bool IsSpaceOnBoard(POINT space)
{
    return
        space.x >= 0 && space.x < 8 &&
        space.y >= 0 && space.y < 8;
}

HBRUSH CheckerBrush(int i, HBRUSH white, HBRUSH black)
{
    POINT space = IndexToBoard(i);
    return (space.x + space.y) & 1 ? white : black;
}
HBRUSH CheckerBrush(int x, int y, HBRUSH white, HBRUSH black)
{
    return (x + y) & 1 ? white : black;
}

void DrawBoardSpace(int space_x, int space_y, HBRUSH backgroundColor, bool animate)
{
    if (!IsSpaceOnBoard(space_x, space_y))
        return;

    int index = BoardToIndex(space_x, space_y);
    BoardToScreen(&space_x, &space_y);

    SelectObject(g_hdc, backgroundColor);
    Rectangle(g_hdc, space_x, space_y, space_x + named::g_spaceWidth + 1, space_y + named::g_spaceWidth + 1);

    if (!!g_board[index])
        (animate ? DrawVectorGraphicAnimated : DrawVectorGraphic)(*(g_board[index]->GetGraphic()), space_x, space_y, g_board[index]->team);
}
void DrawBoardSpace(int index, HBRUSH backgroundColor, bool animate)
{
    POINT space = IndexToBoard(index);
    DrawBoardSpace(space.x, space.y, backgroundColor, animate);
}

int main()
{
    HWND hWnd = GetConsoleWindow();
    g_hdc = GetDC(hWnd);

    HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
    SelectObject(g_hdc, hPen);

    HBRUSH blackBrush = CreateSolidBrush(g_palette[1][5]);
    HBRUSH whiteBrush = CreateSolidBrush(g_palette[0][5]);
    HBRUSH select = CreateSolidBrush(RGB(0, 50, 200));

    POINT cursor = { 0,0 }; // Location of the mouse (space changes as needed)
    POINT space = cursor; // Most recent space the cursor was hovering
    int selectedSpace = -1; // Index of the space selected
    bool downDirty = true; // A bit of a hack because I can't figure out how to test when the mouse is pressed, only when it's down.
    int turn = 0;

    // Draw board spaces
    for (int i = 0; i < 64; ++i)
    {
        POINT space = IndexToBoard(i);
        DrawBoardSpace(space.x, space.y, CheckerBrush(space.x, space.y, whiteBrush, blackBrush), false);
    }

    // Game loop
    while (true)
    {
        GetCursorPos(&cursor);
        ScreenToClient(hWnd, &cursor);
        ScreenToBoard(&cursor);

        if (IsSpaceOnBoard(cursor))
        {
            bool b_moved = (space.x != cursor.x || space.y != cursor.y);

            // Mouse moved to a new space
            if (b_moved)
            {
                DrawBoardSpace(space.x, space.y, CheckerBrush(space.x, space.y, whiteBrush, blackBrush), false);
                space = cursor;
            }

            // Get space index
            int i = BoardToIndex(cursor);

            // Reset the animation
            if (b_moved && !!g_board[i])
                g_board[i]->GetGraphic()->Anim_Zero();

            // Select clicked space
            if (GetKeyState(VK_LBUTTON) & 0x80 && downDirty)
            {
                downDirty = false;

                // Clean previously selected space
                if (selectedSpace != -1)
                {
                    if (selectedSpace == i) // Click selected space
                        selectedSpace = -1;
                    else
                    {
                        POINT selected = IndexToBoard(selectedSpace);
                        DrawBoardSpace(selected.x, selected.y, CheckerBrush(selected.x, selected.y, whiteBrush, blackBrush), false);
                        selectedSpace = i; // Select current space
                    }
                }
                else
                    selectedSpace = i; // Select current space
            }
            else if (!(GetKeyState(VK_LBUTTON) & 0x80))
                downDirty = true;

            // Draw the current space
            if (selectedSpace == i)
                DrawBoardSpace(cursor.x, cursor.y, select, true);
            else
            {
                if (selectedSpace != -1)
                    DrawBoardSpace(selectedSpace, select, true);

                DrawBoardSpace(cursor.x, cursor.y, CheckerBrush(i, whiteBrush, blackBrush), true);
            }

            if (selectedSpace != -1)
                Animation::agitation = 1.0f;
            else
                Animation::agitation = 0.0f;

            // Update animation (if there is a unit to update the animation of)
            Animation::Tick();
            if (Animation::t == 0.0f)
            {
                if (!!g_board[i])
                    g_board[i]->GetGraphic()->Anim_Generate();

                if (selectedSpace != -1 && !!g_board[selectedSpace] && g_board[selectedSpace] != g_board[i])
                    g_board[selectedSpace]->GetGraphic()->Anim_Generate();
            }
        }

        Sleep(16);
    }

    // Cleanup

    DeleteObject(blackBrush);
    DeleteObject(whiteBrush);
    DeleteObject(select);

    DeleteObject(hPen);

    GetStockObject(WHITE_BRUSH);
    GetStockObject(DC_PEN);

    ReleaseDC(hWnd, g_hdc);

    return 0;
}