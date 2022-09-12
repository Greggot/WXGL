#pragma once
#include <string>
#include <list>
#include <gl/GL.h>

struct vertex
{
    float x;
    float y;
    float z;

    vertex() : x(0), y(0), z(0) {}
    vertex(float single) : x(single), y(single), z(single) {}
    vertex(float x, float y, float z) : x(x), y(y), z(z) {}

    vertex& operator+=(const vertex& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    vertex& operator-=(const vertex& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    inline void draw() const { glVertex3f(x, y, z); }
    inline float max() const { return std::max({ x, y, z }); }
};

typedef vertex color;
static const color standardColor(0.5);

struct move {
    vertex T;
    vertex R;
};

class BaseModel
{
protected:
    const std::string Path;

    static inline color GradientStep(color original, size_t steps) {
        return color(original.max() / steps);
    }

    virtual void ActiveOutlineDraw() const {};
    static inline void setColorFrom(uint32_t ID) {
        static GLuint Color[3] = { 0 };
        Color[2] = ID & 0xFF;
        ID >>= 8;
        Color[1] = ID & 0xFF;
        ID >>= 8;
        Color[0] = ID & 0xFF;
        glColor3ub(Color[0], Color[1], Color[2]);
    }

    BaseModel* parent = nullptr;
    std::list<BaseModel*> children;

    inline void ApplyMovementFromBottomToTop() const {
	    BaseModel* head = parent;
        std::list<move> Moves;
	    if (parent)
	    {
            do
            {
                Moves.push_front({ head->Translation, head->Rotation });
                head = head->parent;
            }
            while (head);

            for (auto move : Moves)
                ApplyMovement(move.T, move.R);
	    }
    }
    // TODO: Replace Euler angles with quaternions
    void ApplyMovement(const vertex& T, const vertex& R) const {
        glTranslatef(T.x, T.y, T.z);
        glRotatef(R.x, 1.0, 0.0, 0.0);
        glRotatef(R.y, 0.0, 1.0, 0.0);
        glRotatef(R.z, 0.0, 0.0, 1.0);
    }
    void ApplyMovement() const
    {
        ApplyMovementFromBottomToTop();
        ApplyMovement(Translation, Rotation);
    }
public:
    const std::string Name;
    
    BaseModel() {}
    BaseModel(const char path[])
        : Path(std::string(path)), Name(Path.substr(Path.find_last_of("\\") + 1))
    {}

    vertex Translation;
    vertex Rotation;
    float Scale = 1;
    bool Active = false;

    void LinkTo(BaseModel* Parent) { 
        if (parent)
            RemoveFromTree();
        parent = Parent; 
        parent->children.push_back(this);
    }
    void RemoveFromTree() {
        if (parent == nullptr)
            return;

        parent->children.remove(this);
        for (auto child : children)
            child->LinkTo(parent);
    }

    virtual void Draw() const = 0;
    virtual void ColorSelectDraw(uint32_t ID) const {};
    static uint32_t GetColorSelection(uint32_t x, uint32_t y)
    {
        GLint viewport[4] = { 0 };
        glGetIntegerv(GL_VIEWPORT, viewport);

        GLubyte Color[4] = { 0 };
        glReadPixels(x, viewport[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, Color);

        return (Color[0] << 16) | (Color[1] << 8) | Color[2];
    }

    virtual ~BaseModel() {};
};
