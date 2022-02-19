#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>

struct Vector3
{
    float x, y, z;
};
constexpr Vector3 Vector3Zero()
{
    return { 0,0,0 };
}

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

enum class Class
{
    Object,
    Actor,
    GlooProjectile,
    Gloo,
};

std::unordered_map<Class, std::unordered_set<Class>> g_inheritance;

using MemberToken_t = size_t;

enum class MemberType
{
    Boolean,
    Integer,
    Float,
    String,
    Class,
    Container,
    Object,
};
struct MemberContainer;
struct Member
{
    MemberType m_memberType;
    union
    {
        bool m_boolean;
        int m_boolean;
        float m_float;
        std::string m_string;
        Class m_class;
        Container m_container;
        struct { Class storableClass; Object* referenece; } m_object;
    };
};
enum class ContainerType;
struct Container
{
    ContainerType m_containerType;
    union
    {

    };
};

class Object
{
private:
    Class m_class;
    std::unordered_map<MemberToken_t, Member> m_members;

public:
    bool HasClassInheritance(Class with) const
    {

    }
    Class GetClass() const
    {
        return m_class;
    }

    friend Object* Spawn(Class what, Transform where);
};
std::vector<Object*> g_world;
Object* Spawn(Class what, Transform where)
{
    Object* object = new Object;
    g_world.push_back();
}

int main()
{
    return 0;
}
