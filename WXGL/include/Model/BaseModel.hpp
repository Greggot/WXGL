#pragma once
#include <string>
#include <list>
#include <algorithm>

#include <windows.h>
#include <gl/GL.h>

#include <vertex.h>
#include <SkyBlue/Types.hpp>

enum class axis_t { x, y, z };
enum class angles_t { x, y, z };

class ModuleModel
{
private:
    union DegreeOfFreedom {
        uint8_t raw = 0xFF;
        struct
        {
            uint8_t x : 1;
            uint8_t y : 1;
            uint8_t z : 1;
            
            uint8_t alpha : 1;
            uint8_t beta  : 1;
            uint8_t gamma : 1;

            uint8_t reserved : 2;
        };
    };
    SkyBlue::ID id;
protected:
    DegreeOfFreedom degree;
public:
    void Change(SkyBlue::ID id) { this->id = id; }

    DegreeOfFreedom GetDegreeOfFreedom() const { return degree; }
    void Set(DegreeOfFreedom degree) { this->degree = degree; }
};

class MovableModel : public ModuleModel
{
protected:
    vertex Translation;
    vertex Rotation;
    
    void SetMovement(const vertex& r, const vertex& t) const {
        glTranslatef(t.x, t.y, t.z);
        glRotatef(r.x, 1, 0, 0);
        glRotatef(r.y, 0, 1, 0);
        glRotatef(r.z, 0, 0, 1);
    }
public:
    void Move(axis_t axis, double value){
        int index = static_cast<int>(axis);
        if (degree.raw & (1 << index))
            Translation[index] = value;
    };
    void Rotate(angles_t angle, double value){
        int index = static_cast<int>(angle);
        if (degree.raw & (8 << index))
            Rotation[index] = value;
    }

    vertex GetTranslation() const { return Translation; }
    vertex GetRotation() const { return Rotation; }
    
    void ApplyOwnMovement() const {
        glTranslatef(Translation.x, Translation.y, Translation.z);
        glRotatef(Rotation.x, 1, 0, 0);
        glRotatef(Rotation.y, 0, 1, 0);
        glRotatef(Rotation.z, 0, 0, 1);
    }
};

class DependencyNode : public MovableModel
{
private:
    // parent == nullptr <=> model is independent
    DependencyNode* parent = nullptr;
    std::list<DependencyNode*> children;
    
    struct move {
        vertex T;
        vertex R;
    };
protected:
    inline void ApplyHierarchyMovement() const {
        auto head = parent;
        std::list<move> Moves;
        if (parent)
        {
            do
            {
                Moves.push_front({ head->Translation, head->Rotation });
                head = head->parent;
            } while (head);

            for (const move& m : Moves)
                SetMovement(m.T, m.R);
        }
    }
public:
    void LinkTo(DependencyNode* node) {
        parent = node;
        if(parent)
            parent->children.push_back(this);
    }

    void AddChild(DependencyNode* node) {
        if (node == nullptr)
            return;
        children.push_back(node);
        node->parent = this;
    }

    void RemoveFromTree() {
        if (parent)
            parent->children.remove(this);
        for (auto child : children)
            child->LinkTo(parent);
    }
};

typedef vertex color;
static const color standardColor{ 0.5 };
class DrawableModel : public DependencyNode
{
private:
protected:
    virtual void PolygoneRender() const = 0;
    virtual void OutlineRender() const = 0;

    static inline color GradientStep(color original, size_t steps) {
        return color(original.maxvalue() / steps);
    }
    static void setColorFrom(uint32_t ID) {
        static GLuint Color[3] = { 0 };
        Color[2] = ID & 0xFF;
        ID >>= 8;
        Color[1] = ID & 0xFF;
        ID >>= 8;
        Color[0] = ID & 0xFF;
        glColor3ub(Color[0], Color[1], Color[2]);
    }
    const std::string Path;
public:
    DrawableModel(std::string&& Path) : Path(Path), Name(Path.substr(Path.find_last_of("\\") + 1)) {}

    const std::string Name;
    float Scale = 1;
    bool Active = false;

    virtual void Draw() const {
        glPushMatrix();

        glScalef(Scale, Scale, Scale);
        ApplyHierarchyMovement();
        ApplyOwnMovement();

        PolygoneRender();
        if (Active)
            OutlineRender();

        glPopMatrix();
    };

    virtual void SelectRender(int) = 0;
    static int GetColorSelection(int x, int y) {
        GLint viewport[4] = { 0 };
        glGetIntegerv(GL_VIEWPORT, viewport);
        GLubyte Color[4] = { 0 };
        glReadPixels(x, viewport[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, Color);
        return (Color[0] << 16) | (Color[1] << 8) | Color[2];
    }

    virtual ~DrawableModel() {};
};

class BaseModel
{
private:
    struct move {
        vertex T;
        vertex R;
    };

    BaseModel* parent = nullptr;
    std::list<BaseModel*> children;

protected:
    const std::string Path;

    static inline color GradientStep(color original, size_t steps) {
        return color(original.maxvalue() / steps);
    }
    static void setColorFrom(uint32_t ID);

    // TODO: Replace Euler angles with quaternions
    inline void ApplyMovementFromBottomToTop() const;
    void ApplyMovement(const vertex& T, const vertex& R) const;
    void ApplyMovement() const;

    virtual void ActiveOutlineDraw() const = 0;
    virtual void ConcreteDraw() const = 0;
public:
    const std::string Name;

    vertex Translation;
    vertex Rotation;
    float Scale = 1;
    bool Active = false;

    BaseModel() = default;
    BaseModel(const char path[])
        : Path(std::string(path)), Name(Path.substr(Path.find_last_of("\\") + 1))
    {}

    void Orphan();
    void LinkTo(BaseModel* Parent);
    void RemoveFromTree();

    virtual void Draw() const {
        glPushMatrix();
        
        glScalef(Scale, Scale, Scale);
        ApplyMovement();
        ConcreteDraw();
        if (Active)
            ActiveOutlineDraw();

        glPopMatrix();
    };

    virtual void DrawSelectionMode(uint32_t ID) const {};
    static uint32_t GetColorSelection(uint32_t x, uint32_t y);

    virtual ~BaseModel() {};
};

