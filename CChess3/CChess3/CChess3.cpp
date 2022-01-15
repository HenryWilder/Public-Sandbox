#include <Windows.h>
#include <windowsx.h>
#include <vector>
#include <fstream>

// Named values
namespace named
{
    constexpr LONG g_spaceWidth = 50;
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
        return Lerp(5.0f * ((float)named::g_spaceWidth / 100.0f), 15.0f * ((float)named::g_spaceWidth / 100.0f), agitation) * ((float)(rand() - (RAND_MAX / 2)) / RAND_MAX);
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
    int colorID = -1;
    HBRUSH hBrush = CreateSolidBrush(g_palette[team & 1][0]);
    for (const Triangle& tri : vg.paths)
    {
        if (colorID != tri.colorID)
        {
            colorID = tri.colorID;

            if (colorID != -1)
                DeleteObject(hBrush);

            hBrush = CreateSolidBrush(g_palette[team & 1][colorID]);
            SelectObject(g_hdc, hBrush);
        }

        POINT apt[3];
        for (int i = 0; i < 3; ++i)
        {
            apt[i].x = (tri.apt[i].x + (LONG)x + (LONG)(tri.ptAnim[i][0].Sample() + 0.5f));
            apt[i].y = (tri.apt[i].y + (LONG)y + (LONG)(tri.ptAnim[i][1].Sample() + 0.5f));
        }

        Polygon(g_hdc, apt, 3);
    }
    DeleteObject(hBrush);
}
void DrawVectorGraphic(const VectorGraphic& vg, int x, int y, int team)
{
    int colorID = -1;
    HBRUSH hBrush = CreateSolidBrush(g_palette[team & 1][0]);
    for (const Triangle& tri : vg.paths)
    {
        if (colorID != tri.colorID)
        {
            colorID = tri.colorID;

            if (colorID != -1)
                DeleteObject(hBrush);

            hBrush = CreateSolidBrush(g_palette[team & 1][colorID]);
            SelectObject(g_hdc, hBrush);
        }

        POINT apt[3];
        for (int i = 0; i < 3; ++i)
        {
            apt[i].x = (tri.apt[i].x + (LONG)x);
            apt[i].y = (tri.apt[i].y + (LONG)y);
        }

        Polygon(g_hdc, apt, 3);
    }
    DeleteObject(hBrush);
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

struct SpaceDrawingData
{
    POINT space;
    HBRUSH background;
    Unit* unitAtSpace;
    bool animated;
};

int main()
{
    HWND hWnd = GetConsoleWindow();
    g_hdc = GetDC(hWnd);

    HPEN hPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
    SelectObject(g_hdc, hPen);

    HBRUSH blackBrush = CreateSolidBrush(g_palette[1][5]);
    HBRUSH whiteBrush = CreateSolidBrush(g_palette[0][5]);
    HBRUSH select = CreateSolidBrush(RGB(0, 50, 200));
    HBRUSH highlight = CreateSolidBrush(RGB(200, 200, 0));

    POINT cursor = { 0,0 }; // Location of the mouse (space changes as needed)
    POINT space = cursor; // Most recent space the cursor was hovering
    bool downDirty = true; // A bit of a hack because I can't figure out how to test when the mouse is pressed, only when it's down.
    bool b_mousePressed = false;
    int turn = 0;

    std::vector<int> spacesToClean;
    int hoveredSpace = -1; // Index of the space hovered
    int selectedSpace = -1; // Index of the space selected

    // Draw board
    for (int i = 0; i < 64; ++i)
    {
        POINT space = IndexToBoard(i);
        DrawBoardSpace(space.x, space.y, CheckerBrush(space.x, space.y, whiteBrush, blackBrush), false);
    }

    // Game loop
    while (true)
    {
        // UPDATE VARIABLES

        GetCursorPos(&cursor);
        ScreenToClient(hWnd, &cursor);
        ScreenToBoard(&cursor);

        {
            if (downDirty == false)
                b_mousePressed = false;

            if (GetKeyState(VK_LBUTTON) & 0x80 && downDirty) // On mouse press
            {
                downDirty = false; // Mark that we have handled the current click
                b_mousePressed = true;
            }
            else if (!(GetKeyState(VK_LBUTTON) & 0x80)) // Mouse is up
                downDirty = true; // We are allowed to handle clicks next frame
        }

        // Check if the mouse has moved to a new board space
        bool b_moved = (space.x != cursor.x || space.y != cursor.y);

        bool b_onBoard = IsSpaceOnBoard(cursor);

        if (b_onBoard)
            hoveredSpace = BoardToIndex(cursor);
        else
            hoveredSpace = -1;

        // Mouse moved to a new space
        if (b_moved)
        {
            // Clean up the previous space
            spacesToClean.push_back(BoardToIndex(space));
            space = cursor;

            // Reset the animation for the new space
            if (hoveredSpace != -1 && b_onBoard && !!g_board[hoveredSpace])
                g_board[hoveredSpace]->GetGraphic()->Anim_Zero();
        }

        // Selection
        if (b_mousePressed && b_onBoard) // On mouse press
        {
            if (!!g_board[hoveredSpace] && (g_board[hoveredSpace]->team & 1) == (turn & 1)) // Check if selection is allowed
            {
                if (selectedSpace != -1) // There is already a selection
                {
                    spacesToClean.push_back(selectedSpace); // Clean previously selected space
                    if (selectedSpace == hoveredSpace) // Click selected space
                    {
                        selectedSpace = -1; // Clear the selection
                    }
                    else // The selection is at a different space - piece can move!
                    {
                        selectedSpace = hoveredSpace; // Select current space
                    }
                }
                else // There is no selection
                {
                    selectedSpace = hoveredSpace; // Select current space
                }
            }
            else
            {
                if (selectedSpace != -1)
                    spacesToClean.push_back(selectedSpace); // Clean previously selected space

                selectedSpace = -1;
            }
        }


        // DRAW THE FRAME

        for (int clean : spacesToClean)
        {
            DrawBoardSpace(clean, CheckerBrush(clean, whiteBrush, blackBrush), false);
        }
        spacesToClean.clear();

        if (selectedSpace != -1 && selectedSpace != hoveredSpace)
            DrawBoardSpace(selectedSpace, select, true);

        if (hoveredSpace != -1)
            DrawBoardSpace(hoveredSpace, (selectedSpace != -1 ? (selectedSpace == hoveredSpace ? select : highlight) : CheckerBrush(hoveredSpace, whiteBrush, blackBrush)), true);

        // UPDATE ANIMATION

        // TODO: Make agitation based on whether a piece can move to a space or not
        //if (selectedSpace != -1)
        //    Animation::agitation = 1.0f;
        //else
        //    Animation::agitation = 0.0f;


        Animation::Tick(); // Mark that the frame has passed
        if (Animation::t == 0.0f) // The animation time has been reset
        {
            if (hoveredSpace != -1 && !!g_board[hoveredSpace]) // If there is a piece at the space being hovered
            {
                g_board[hoveredSpace]->GetGraphic()->Anim_Generate(); // Generate the next two animation control points
            }

            if (selectedSpace != -1 && // There is a selection
                !!g_board[selectedSpace] && // It contains a piece
                g_board[hoveredSpace] != g_board[selectedSpace]) // That piece is not the one that just now had its animation updated
            {
                g_board[selectedSpace]->GetGraphic()->Anim_Generate(); // Generate the next two animation control points
            }
        }


        Sleep(16); // Make time go forward
    }

    // Cleanup

    DeleteObject(blackBrush);
    DeleteObject(whiteBrush);
    DeleteObject(select);
    DeleteObject(highlight);

    DeleteObject(hPen);

    GetStockObject(WHITE_BRUSH);
    GetStockObject(DC_PEN);

    ReleaseDC(hWnd, g_hdc);

    return 0;
}