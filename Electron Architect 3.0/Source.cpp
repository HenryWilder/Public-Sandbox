#include <raylib.h>
#include <raymath.h>
#include <vector>

#define sign(x) (((x) > (decltype(x))(0)) - ((x) < (decltype(x))(0)))

using Vector1 = float;

#pragma region Vector math

inline Vector2  operator- (Vector2  vec)          { return      Vector2Negate(vec);             }
inline Vector2  operator+ (Vector2  a, Vector2 b) { return      Vector2Add(a, b);               }
inline Vector2& operator+=(Vector2& a, Vector2 b) { return (a = Vector2Add(a, b));              }
inline Vector2  operator+ (Vector2  a, float val) { return      Vector2AddValue(a, val);        }
inline Vector2& operator+=(Vector2& a, float val) { return (a = Vector2AddValue(a, val));       }
inline Vector2  operator- (Vector2  a, Vector2 b) { return      Vector2Subtract(a, b);          }
inline Vector2& operator-=(Vector2& a, Vector2 b) { return (a = Vector2Subtract(a, b));         }
inline Vector2  operator- (Vector2  a, float val) { return      Vector2SubtractValue(a, val);   }
inline Vector2& operator-=(Vector2& a, float val) { return (a = Vector2SubtractValue(a, val));  }
inline Vector2  operator* (Vector2  a, Vector2 b) { return      Vector2Multiply(a, b);          }
inline Vector2& operator*=(Vector2& a, Vector2 b) { return (a = Vector2Multiply(a, b));         }
inline Vector2  operator* (Vector2  a, float scl) { return      Vector2Scale(a, scl);           }
inline Vector2& operator*=(Vector2& a, float scl) { return (a = Vector2Scale(a, scl));          }
inline Vector2  operator/ (Vector2  a, Vector2 b) { return      Vector2Divide(a, b);            }
inline Vector2& operator/=(Vector2& a, Vector2 b) { return (a = Vector2Divide(a, b));           }
inline Vector2  operator/ (Vector2  a, float div) { return      Vector2Scale(a, 1.0f / div);    }
inline Vector2& operator/=(Vector2& a, float div) { return (a = Vector2Scale(a, 1.0f / div));   }

inline Vector3  operator- (Vector3  vec)          { return      Vector3Negate(vec);             }
inline Vector3  operator+ (Vector3  a, Vector3 b) { return      Vector3Add(a, b);               }
inline Vector3& operator+=(Vector3& a, Vector3 b) { return (a = Vector3Add(a, b));              }
inline Vector3  operator+ (Vector3  a, float val) { return      Vector3AddValue(a, val);        }
inline Vector3& operator+=(Vector3& a, float val) { return (a = Vector3AddValue(a, val));       }
inline Vector3  operator- (Vector3  a, Vector3 b) { return      Vector3Subtract(a, b);          }
inline Vector3& operator-=(Vector3& a, Vector3 b) { return (a = Vector3Subtract(a, b));         }
inline Vector3  operator- (Vector3  a, float val) { return      Vector3SubtractValue(a, val);   }
inline Vector3& operator-=(Vector3& a, float val) { return (a = Vector3SubtractValue(a, val));  }
inline Vector3  operator* (Vector3  a, Vector3 b) { return      Vector3Multiply(a, b);          }
inline Vector3& operator*=(Vector3& a, Vector3 b) { return (a = Vector3Multiply(a, b));         }
inline Vector3  operator* (Vector3  a, float scl) { return      Vector3Scale(a, scl);           }
inline Vector3& operator*=(Vector3& a, float scl) { return (a = Vector3Scale(a, scl));          }
inline Vector3  operator/ (Vector3  a, Vector3 b) { return      Vector3Divide(a, b);            }
inline Vector3& operator/=(Vector3& a, Vector3 b) { return (a = Vector3Divide(a, b));           }
inline Vector3  operator/ (Vector3  a, float div) { return      Vector3Scale(a, 1.0f / div);    }
inline Vector3& operator/=(Vector3& a, float div) { return (a = Vector3Scale(a, 1.0f / div));   }

#pragma endregion

constexpr float g_nodeRadius = 4.0f;

float Vector2DistanceToLine(Vector2 startPos, Vector2 endPos, Vector2 point)
{
    Vector2 direction = Vector2Normalize(endPos - startPos);
    Vector2 nearestPoint = startPos + direction * Clamp(Vector2DotProduct(direction, point - startPos), 0, Vector2Distance(startPos, endPos));
    return Vector2Distance(point, nearestPoint);
}
bool CheckCollisionLineCircle(Vector2 startPos, Vector2 endPos, Vector2 center, float radius)
{
    return Vector2DistanceToLine(startPos, endPos, center) <= radius;
}

int Snap(int x, int gridsize)
{
    x /= gridsize;
    return x * gridsize;
}

Vector2 Snap(Vector2 pt, float gridsize)
{
    return {
        roundf(pt.x / gridsize) * gridsize,
        roundf(pt.y / gridsize) * gridsize
    };
}

class Node
{
private:
    Vector2 m_position;
    bool b_inverts; // Treat this as a NOR gate? If not, treat it as an OR gate
    bool b_state;
    bool b_visited;

    std::vector<Node*> m_inputs;
    std::vector<Node*> m_outputs;

public:
    Node(Vector2 position, bool inverts) : m_position(position), b_inverts(inverts), b_state(false), b_visited(false), m_inputs{}, m_outputs{} {}

    Vector2 GetPosition() const
    {
        return m_position;
    }
    void SetPosition(Vector2 position)
    {
        m_position = position;
    }

    bool GetInverts() const
    {
        return b_inverts;
    }
    void SetInverts(bool inverts)
    {
        b_inverts = inverts;
    }

    bool GetState() const
    {
        return b_state;
    }
    void SetState(bool state)
    {
        b_state = state;
    }

    bool GetVisited() const
    {
        return b_visited;
    }
    void SetVisited(bool visited)
    {
        b_visited = visited;
    }

    bool HasNoInputs() const
    {
        return m_inputs.empty();
    }
    bool InputCount() const
    {
        return m_inputs.size();
    }
    const std::vector<Node*>& GetInputs() const
    {
        return m_inputs;
    }
    void AddInput(Node* input)
    {
        if (!input)
            return;

        if (std::find(m_inputs.begin(), m_inputs.end(), input) == m_inputs.end()) // Prevent duplicates
            m_inputs.push_back(input);
    }
    void RemoveInput(const Node* input)
    {
        if (!input)
            return;

        auto it = std::find(m_inputs.begin(), m_inputs.end(), input);
        if (it != m_inputs.end())
            m_inputs.erase(it);
    }

    bool HasNoOutputs() const
    {
        return m_outputs.empty();
    }
    bool OutputCount() const
    {
        return m_outputs.size();
    }
    const std::vector<Node*>& GetOutputs() const
    {
        return m_outputs;
    }
    void AddOutput(Node* output)
    {
        if (!output)
            return;

        if (std::find(m_outputs.begin(), m_outputs.end(), output) == m_outputs.end()) // Prevent duplicates
            m_outputs.push_back(output);
    }
    void RemoveOutput(const Node* output)
    {
        if (!output)
            return;

        auto it = std::find(m_outputs.begin(), m_outputs.end(), output);
        if (it != m_outputs.end())
            m_outputs.erase(it);
    }

    // Warning: Affects input/output nodes
    void ClearReferencesToSelf()
    {
        for (Node* input : m_inputs)
        {
            input->RemoveOutput(this);
        }
        m_inputs.clear();

        for (Node* output : m_outputs)
        {
            output->RemoveInput(this);
        }
        m_outputs.clear();
    }
};

struct Wire
{
    Node* a;
    Node* b;
};

struct Graph
{
    size_t seedCount = 0;
    // Sort using CalculateRoute() every time a node is added or removed
    std::vector<Node*> nodes;

    // Runs every tick
    void Step()
    {
        for (Node* node : nodes)
        {
            bool finished = false;
            for (Node* input : node->GetInputs())
            {
                if (input->GetState())
                {
                    node->SetState(!node->GetInverts());
                    finished = true;
                    break;
                }
            }
            if (!finished)
                node->SetState(node->GetInverts());
        }
    }
    // Runs every frame
    void DrawWires()
    {
        // Draw the wires first so the nodes can be drawn on top
        for (Node* node : nodes)
        {
            for (Node* output : node->GetOutputs())
            {
                DrawLineV(node->GetPosition(), output->GetPosition(), (node->GetState() ? DARKBLUE : DARKGRAY));
            }
        }
    }
    void DrawNodes()
    {
        // Node drawing is separate so that cyclic graphs can still draw nodes on top
        for (Node* node : nodes)
        {
            DrawCircleV(node->GetPosition(), g_nodeRadius + 2.0f, (node->GetInverts() ? ORANGE : PURPLE));
            DrawCircleV(node->GetPosition(), g_nodeRadius, (node->GetState() ? BLUE : BLACK));
            if (node->HasNoInputs())
                DrawCircleV(node->GetPosition(), g_nodeRadius - 2.0f, GREEN);
        }
    }

    void ResetVisited()
    {
        for (Node* node : nodes)
        {
            node->SetVisited(false);
        }
    }

    void ResetStates()
    {
        for (Node* node : nodes)
        {
            node->SetState(node->GetInverts());
        }
    }

    bool FindSeeds(std::vector<Node*>* buff, Node* node)
    {
        bool foundSeed = node->HasNoInputs();
        if (foundSeed)
            buff->push_back(node);

        node->SetVisited(true);

        for (Node* output : node->GetOutputs())
        {
            if (!output->GetVisited())
                foundSeed = FindSeeds(buff, output) || foundSeed;
        }
        for (Node* input : node->GetInputs())
        {
            if (!input->GetVisited())
                foundSeed = FindSeeds(buff, input) || foundSeed;
        }
        return foundSeed;
    }

    // Uses modified BFS
    // Make sure to run this before simulating a step when the graph has changed
    void CalculateRoute()
    {
        ResetVisited();

        size_t totalNodes = nodes.size();

        // Put the "seed" nodes at the start
        {
            std::vector<Node*> buff;
            buff.reserve(totalNodes);
            for (Node* node : nodes)
            {
                if (!node->GetVisited())
                {
                    bool found = FindSeeds(&buff, node);
                    if (!found)
                        buff.push_back(node);
                }
            }
            nodes.swap(buff);
        }
        seedCount = nodes.size(); // Only the seeds will be in at this time

        ResetVisited();

        size_t startIndex = 0;
        size_t endIndex;
        while (nodes.size() < totalNodes)
        {
            endIndex = nodes.size();
            for (size_t i = startIndex; i < endIndex; ++i)
            {
                for (Node* output : nodes[i]->GetOutputs())
                {
                    if (!!output->GetVisited())
                    {
                        nodes.push_back(output);
                        output->SetVisited(!true);
                    }
                }
            }
            startIndex = endIndex;
        }
    }

    // Create a node without connections
    void AddNode(Node* newNode)
    {
        if (!newNode)
            return;

        if (std::find(nodes.begin(), nodes.end(), newNode) == nodes.end())
            nodes.push_back(newNode);
    }

    void ConnectNodes(Node* start, Node* end)
    {
        auto endIt   = std::find(start->GetInputs() .begin(), start->GetInputs() .end(), end  );
        auto startIt = std::find(end  ->GetOutputs().begin(), end  ->GetOutputs().end(), start);

        if (!(endIt   == start->GetInputs() .end() &&
              startIt == end  ->GetOutputs().end()))
            return;

        start->AddOutput(end  );
        end  ->AddInput (start);
    }
    void DisconnectNodes(Node* start, Node* end)
    {
        start->RemoveOutput(end);
        end->RemoveInput(start);
    }

    // Erases a node from existence
    void RemoveNode(Node* node)
    {
        if (!node)
            return;

        auto it = std::find(nodes.begin(), nodes.end(), node);
        if (it != nodes.end())
        {
            node->ClearReferencesToSelf();
            nodes.erase(it);

            CalculateRoute();
        }
    }

    Node* FindNodeAtPosition(Vector2 position, float radius)
    {
        float shortestDistance = radius;
        Node* closest = nullptr;
        for (Node* node : nodes)
        {
            float distance = Vector2Distance(position, node->GetPosition());
            if (distance < shortestDistance)
            {
                shortestDistance = distance;
                closest = node;
            }
        }
        return closest;
    }

    Wire FindWireIntersectingPosition(Vector2 position, float radius)
    {
        float shortestDistance = radius;
        Wire closest = { nullptr, nullptr };
        ResetVisited();
        for (Node* start : nodes)
        {
            if (start->GetVisited())
                continue;

            for (Node* end : start->GetOutputs())
            {
                if (end->GetVisited())
                    break;

                float distance = Vector2DistanceToLine(start->GetPosition(), end->GetPosition(), position);
                if (distance < shortestDistance)
                {
                    shortestDistance = distance;
                    closest = { start, end };
                }
            }
        }
        return closest;
    }
};

int main()
{
    int windowWidth = 1280;
    int windowHeight = 720;
    InitWindow(windowWidth, windowHeight, "My Raylib Program");
    SetTargetFPS(60);

    /******************************************
    *   Load textures, shaders, and meshes    *
    ******************************************/

    bool graphDirty = false;
    bool gateDirty = false;
    Graph graph;

    Node* selectionStart = nullptr;
    Node* selectionEnd = nullptr;

    while (!WindowShouldClose())
    {
        /******************************************
        *   Simulate frame and update variables   *
        ******************************************/

        Vector2 cursor = Snap(GetMousePosition(), g_nodeRadius * 2.0f);
        if (selectionStart)
        {
            float xSize = abs(selectionStart->GetPosition().x - cursor.x);
            float ySize = abs(selectionStart->GetPosition().y - cursor.y);
            float shortLength = std::min(xSize, ySize);
            float longLength = std::max(xSize, ySize);
            if (shortLength > longLength * 0.5)
            {
                cursor = {
                    selectionStart->GetPosition().x + (selectionStart->GetPosition().x > cursor.x ? -shortLength : shortLength),
                    selectionStart->GetPosition().y + (selectionStart->GetPosition().y > cursor.y ? -shortLength : shortLength)
                };
            }
            else
            {
                if (xSize < ySize)
                    cursor.x = selectionStart->GetPosition().x;
                else
                    cursor.y = selectionStart->GetPosition().y;
            }
        }

        Node* hoveredNode = graph.FindNodeAtPosition(cursor, g_nodeRadius * 2.0f);
        Wire hoveredWire = (hoveredNode ? Wire{ nullptr, nullptr }  : graph.FindWireIntersectingPosition(cursor, g_nodeRadius * 2.0f));

        if (selectionStart)
        {
            selectionEnd = hoveredNode;

            // Finalize
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                if (selectionEnd != selectionStart)
                {
                    if (!selectionEnd)
                    {
                        selectionEnd = new Node(cursor, false);
                        graph.AddNode(selectionEnd);
                    }
                    graph.ConnectNodes(selectionStart, selectionEnd);
                    graphDirty = true;
                }
                selectionStart = selectionEnd = nullptr;
            }
        }
        else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            selectionStart = hoveredNode;

            if (!selectionStart)
            {
                selectionStart = new Node(cursor, false);
                graph.AddNode(selectionStart);
                graphDirty = true;
            }
        }

        if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
        {
            if (hoveredNode)
            {
                hoveredNode->SetInverts(!hoveredNode->GetInverts());
                if (!hoveredNode->HasNoInputs()) // Inputless nodes take output from the user and therefore are treated as mutable
                    gateDirty = true;
            }
        }
        
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        {
            if (hoveredNode)
            {
                graph.RemoveNode(hoveredNode);
                delete hoveredNode;
                hoveredNode = nullptr;
                graphDirty = true;
            }
            else if (hoveredWire.a)
            {
                graph.DisconnectNodes(hoveredWire.a, hoveredWire.b);
                hoveredWire = { nullptr, nullptr };
                graphDirty = true;
            }
        }

        if (graphDirty)
        {
            graph.CalculateRoute();
            graphDirty = false;
        }

        if (gateDirty)
        {
            graph.ResetStates();
            gateDirty = false;
        }

        graph.Step();

        /******************************************
        *   Draw the frame                        *
        ******************************************/

        BeginDrawing(); {

            ClearBackground(BLACK);

            constexpr int gridRadius = 4;
            for (float x = 0.0f; x < (float)windowWidth; x += 2.0f * g_nodeRadius)
            {
                DrawLineV({ x, 0 }, { x, (float)windowHeight }, ColorAlpha(DARKGRAY, 0.125f));
            }
            for (float y = 0.0f; y < (float)windowHeight; y += 2.0f * g_nodeRadius)
            {
                DrawLineV({ 0.0f, y }, { (float)windowWidth, y }, ColorAlpha(DARKGRAY, 0.125f));
            }
            for (float i = -gridRadius; i <= gridRadius; ++i)
            {
                float t = ((i / (float)gridRadius) + 1.0f) * 0.5f;
                float a = g_nodeRadius * 2.0f * sinf(t * PI) * gridRadius;
                float b = g_nodeRadius * 2.0f * i;
                DrawLineV({ cursor.x - a, cursor.y + b }, { cursor.x + a, cursor.y + b }, ColorAlpha(DARKGRAY, 1.0f - abs(i / (float)gridRadius)));
                DrawLineV({ cursor.x + b, cursor.y - a }, { cursor.x + b, cursor.y + a }, ColorAlpha(DARKGRAY, 1.0f - abs(i / (float)gridRadius)));
            }

            if (selectionStart)
            {
                Vector2 end;
                if (selectionEnd)
                    end = selectionEnd->GetPosition();
                else
                    end = cursor;

                DrawLineV(selectionStart->GetPosition(), end, GRAY);
            }

            graph.DrawWires();

            if (hoveredWire.a)
                DrawLineV(hoveredWire.a->GetPosition(), hoveredWire.b->GetPosition(), LIGHTGRAY);

            graph.DrawNodes();

            if (hoveredNode)
            {
                DrawRectanglePro({ cursor.x,cursor.y, g_nodeRadius * 4.0f, g_nodeRadius * 4.0f }, { g_nodeRadius * 2.0f, g_nodeRadius * 2.0f }, 45, WHITE);
                DrawRectanglePro({ cursor.x,cursor.y, g_nodeRadius * 3.0f, g_nodeRadius * 3.0f }, { g_nodeRadius * 1.5f, g_nodeRadius * 1.5f }, 45, BLACK);

                DrawCircleV(hoveredNode->GetPosition(), g_nodeRadius + 2.0f, (hoveredNode->GetInverts() ? ORANGE : PURPLE));
            }
            else
            {
                DrawRectanglePro({ cursor.x,cursor.y,5,5 }, { 2.5,2.5 }, 45, RAYWHITE);
            }

        } EndDrawing();
    }

    /******************************************
    *   Unload and free memory                *
    ******************************************/

    for (Node* node : graph.nodes)
    {
        delete node;
    }

    CloseWindow();

	return 0;
}
