#include <algorithm>
#include <string>
#include <vector>
#include <stack>
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

std::unordered_map<Class, std::unordered_set<Class> /* ancestors, not children */> g_hierarchy;

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
    Container()
    {
        m_containerType = (ContainerType)0;
        m_map.keyType = (MemberType)0;
        m_map.valueType = (MemberType)0;
        m_map.elements = {};
    }
    // Copies the template, not the stored elements
    Container(const Container& containerTemplate)
    {
        m_containerType = containerTemplate.m_containerType;
        switch (m_containerType)
        {
        case ContainerType::Array:
            m_array.type = containerTemplate.m_array.type;
            m_array.elements = {};
            break;
        case ContainerType::Set:
            m_set.type = containerTemplate.m_set.type;
            m_set.elements = {};
            break;
        case ContainerType::Map:
            m_map.keyType = containerTemplate.m_map.keyType;
            m_map.valueType = containerTemplate.m_map.valueType;
            m_map.elements = {};
            break;
        }
    }
    Container(ContainerType container, MemberType typeK, MemberType typeV = (MemberType)0)
    {
        m_containerType = container;
        switch (m_containerType)
        {
        case ContainerType::Array:
            m_array.type = typeK;
            m_array.elements = {};
            break;
        case ContainerType::Set:
            m_set.type = typeK;
            m_set.elements = {};
            break;
        case ContainerType::Map:
            m_map.keyType = typeK;
            m_map.valueType = typeV;
            m_map.elements = {};
            break;
        }
    }
    ~Container() = default;

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

void CopyContainer(Container& dest, const Container& src)
{
    dest.m_containerType = src.m_containerType;
    switch (dest.m_containerType)
    {
    case ContainerType::Array:
        dest.m_array.type = src.m_array.type;
        dest.m_array.elements = src.m_array.elements;
        break;
    case ContainerType::Set:
        dest.m_set.type = src.m_set.type;
        dest.m_set.elements = src.m_set.elements;
        break;
    case ContainerType::Map:
        dest.m_map.keyType = src.m_map.keyType;
        dest.m_map.valueType = src.m_map.valueType;
        dest.m_map.elements = src.m_map.elements;
        break;
    }
}

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
    Member(MemberType type)
    {
        m_type = type;
        switch (type)
        {
        case MemberType::Boolean:
            break;
        case MemberType::Integer:
            break;
        case MemberType::Float:
            break;
        case MemberType::String:
            break;
        case MemberType::Class:
            break;
        case MemberType::Container:
            break;
        case MemberType::Object:
            break;
        default:
            break;
        }
    }
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
std::unordered_map<Class, std::vector<PureMember>> g_classTemplate;

bool DoesInheritFromClass(Class check, Class ancestor)
{
    auto it = g_hierarchy.find(check);
    return it->second.find(ancestor) != it->second.end();
}

class Object;
std::vector<Object*> g_world;

class Object
{
private:
    Class m_class;
    std::unordered_map<MemberToken_t, Member> m_members;

public:
    Object(Class what)
    {
        m_class = what;
        auto it = g_hierarchy.find(what);
        std::vector<Class> lineage(it->second.begin(), it->second.end());
        size_t spaceNeeded = 0;
        for (Class ancestor : lineage)
        {
            spaceNeeded += g_classTemplate.find(ancestor)->second.size();
        }
        m_members.reserve(spaceNeeded);
        for (Class ancestor : lineage)
        {
            for (PureMember member : g_classTemplate.find(ancestor)->second)
            {
                RawMember value;
                m_members.insert(member.m_token, Member{ member.m_type, value });
            }
        }
    }

    Class GetClass() const
    {
        return m_class;
    }
};

int main()
{
    return 0;
}
