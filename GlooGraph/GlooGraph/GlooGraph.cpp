#include <algorithm>
#include <fstream>

struct Vector3
{
    float x, y, z;
};

Vector3 Vector3Add(Vector3 a, Vector3 b)
{
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}
Vector3 Vector3Subtract(Vector3 a, Vector3 b)
{
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}
Vector3 Vector3Multiply(Vector3 a, Vector3 b)
{
    return { a.x * b.x, a.y * b.y, a.z * b.z };
}
Vector3 Vector3Divide(Vector3 a, Vector3 b)
{
    return { a.x / b.x, a.y / b.y, a.z / b.z };
}
Vector3 Vector3Scale(Vector3 vec, float scale)
{
    return { vec.x * scale, vec.y * scale, vec.z * scale };
}
float Vector3Length(Vector3 vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
Vector3 Vector3Normalize(Vector3 vec)
{
    float length = Vector3Length(vec);
    return { vec.x / length, vec.y / length, vec.z / length };
}
float Vector3Distance(Vector3 a, Vector3 b)
{
    return Vector3Length(Vector3Subtract(b, a));
}

struct Transform
{
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
};

class Actor
{
    Transform transform;
};

// This is the only one I'm programming because Unreal already gives you the others for free -3-
struct SphereCollider
{
    Vector3 center;
    float radius;
};
bool CheckCollisionSpheres(const SphereCollider& check, const SphereCollider& against)
{
    if (check.center)
}

class GlooProjectile : public Actor
{
    Vector3 velocity;
};

class Gloo : public Actor
{
    Actor* parent;
};

int main()
{
    return 0;
}
