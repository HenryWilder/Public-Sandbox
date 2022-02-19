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
struct RawMember;
enum class ContainerType
{
    Array,
    Set,
    Map,
};
struct Container
{
    ContainerType m_containerType;
    union
    {
        struct {
            MemberType type;
            std::vector<RawMember*> elements;
        } m_array;

        struct {
            MemberType type;
            std::unordered_set<RawMember*> elements;
        } m_set;

        struct {
            MemberType keyType;
            MemberType valueType;
            std::unordered_map<RawMember*, RawMember*> elements;
        } m_map;
    };
};
struct RawMember
{
    union
    {
        bool m_boolean;
        int m_integer;
        float m_float;
        std::string m_string;
        Class m_class;
        Container m_container;
        struct { Class storableClass; Object* referenece; } m_object;
    };
};
struct Member
{
    MemberType m_type;
    RawMember m_value;
};

// Like an internal name for members
using MemberToken_t = std::string;

// For templating members off of when spawning an object
struct PureMember
{
    MemberToken_t m_token;
    MemberType m_type;
    MemberType m_containerType_key; // Use as "type" in non-map containers
    MemberType m_containerType_value;
};
std::unordered_map<Class, std::vector<PureMember>> g_hierarchy;

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
