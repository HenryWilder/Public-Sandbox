struct Vector3
{
    float x, y, z;
};

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

class GlooProjectile : public Actor
{

};

class Gloo : public Actor
{
    Actor* parent;
};

int main()
{
    return 0;
}
