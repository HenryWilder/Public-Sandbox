#include <algorithm>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
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
    // Ordered by size
    Boolean,
    Byte,
    Integer,
    Float,
    Class,
    Container,
    Object,
    String,
};
struct Container;
struct RawMember
{
    RawMember() { m_string = ""; }
    RawMember(const RawMember& other) { this->m_string = other.m_string; }
    ~RawMember() {}

    RawMember& operator=(const RawMember& other) { this->m_string = other.m_string; }

    union
    {
        bool m_boolean;
        char m_byte;
        int m_integer;
        float m_float;
        Class m_class;
        Container* m_container; // MUST be handled by wrappers!!
        struct { Class storableClass; Object* referenece; } m_object;
        std::string m_string;
    };
};
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
    // Copies the template, not the stored elements
    Container& operator=(const Container& containerTemplate)
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
        return *this;
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
    ~Container()
    {
        switch (m_containerType)
        {
        case ContainerType::Array:
            if (m_array.type == MemberType::Container)
            {
                for (size_t i = 0; i < m_array.elements.size(); ++i)
                {
                    delete m_array.elements[i].m_container;
                }
            }
            break;

        case ContainerType::Set:
            if (m_set.type == MemberType::Container)
            {
                std::vector<RawMember> elements(m_set.elements.begin(), m_set.elements.end());
                for (size_t i = 0; i < elements.size(); ++i)
                {
                    delete elements[i].m_container;
                }
            }
            break;

        case ContainerType::Map:
        {
            if (m_map.valueType == MemberType::Container)
            {
                for (auto it = m_map.elements.begin(); it != m_map.elements.end(); ++it)
                {
                    delete it->second.m_container;
                }
            }
        }
            break;
        }
    }

    ContainerType m_containerType;
    union
    {
        struct {
            MemberType type;
            std::vector<RawMember> elements;
        } m_array;

        struct {
            MemberType type;
            std::set<RawMember> elements;
        } m_set;

        struct {
            MemberType keyType; // Key type is NOT ALLOWED to be a container, due to containers being *pointers* and not the sum of their elements!!
            MemberType valueType;
            std::map<RawMember, RawMember> elements;
        } m_map;
    };
};

struct Member
{
    Member() = default;
    Member(MemberType type)
    {
        m_type = type;
        switch (type)
        {
        case MemberType::Boolean: m_value.m_boolean = false; break;
        case MemberType::Integer: m_value.m_integer = 0; break;
        case MemberType::Float: m_value.m_float = 0.0f; break;
        default: __fallthrough;
        case MemberType::String: m_value.m_string = ""; break;
        case MemberType::Class: m_value.m_class = Class::Object; break;
        case MemberType::Container: m_value.m_container = new Container(); break; // Calls new
        case MemberType::Object: m_value.m_object = { Class::Object, nullptr }; break;
        }
    }
    ~Member()
    {
        if (m_type == MemberType::Container)
            delete m_value.m_container; // Calls delete
    }
    MemberType m_type;
    RawMember m_value;
};

// Returns false on error
bool ArrayAdd(Container container, Member element)
{
    if (container.m_containerType != ContainerType::Array ||
        element.m_type != container.m_array.type)
        return false;

    container.m_array.elements.push_back(element.m_value);
    return true;
}

// Returns false on error
bool SetInsert(Container container, Member element)
{
    if (container.m_containerType != ContainerType::Set ||
        element.m_type != container.m_array.type)
        return false;

    container.m_set.elements.insert(element.m_value);
    return true;
}

// Returns false on error
bool MapInsert(Container container, Member key, Member value)
{
    if (container.m_containerType != ContainerType::Map ||
        key.m_type != container.m_map.keyType ||
        value.m_type != container.m_map.valueType)
        return false;

    container.m_map.elements.insert({ key.m_value, value.m_value });
    return true;
}

// Like an internal name for members
using MemberToken_t = std::string;

// For templating members off of when spawning an object
struct PureMember
{
    MemberToken_t m_token;
    MemberType m_mType;
    ContainerType m_cType;
    union
    {
        MemberType m_cTypeU; // Unary
        struct { MemberType key; MemberType value; } m_cTypeB; // Binary
    };
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
                Member insertion;
                if (member.m_mType == MemberType::Container)
                {
                    insertion.m_type = MemberType::Container;
                    insertion.m_value.m_container = new Container;
                    insertion.m_value.m_container->m_containerType = member.m_cType;
                    switch (member.m_cType)
                    {
                    case ContainerType::Array:
                        insertion.m_value.m_container->m_array.type = member.m_cTypeU;
                        insertion.m_value.m_container->m_array.elements = {};
                        break;
                    case ContainerType::Set:
                        insertion.m_value.m_container->m_set.type = member.m_cTypeU;
                        insertion.m_value.m_container->m_set.elements = {};
                        break;
                    case ContainerType::Map:
                        insertion.m_value.m_container->m_map.keyType = member.m_cTypeB.key;
                        insertion.m_value.m_container->m_map.valueType = member.m_cTypeB.value;
                        insertion.m_value.m_container->m_map.elements = {};
                        break;
                    }
                }
                else
                {
                    insertion = Member(member.m_mType);
                }
                m_members.insert({ member.m_token, insertion });
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
