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
    
    void SetMovement(const vertex& t, const vertex& r) const {
        glTranslatef(t.x, t.y, t.z);
        glRotatef(r.x, 1, 0, 0);
        glRotatef(r.y, 0, 1, 0);
        glRotatef(r.z, 0, 0, 1);
    }
public:
    void Move(axis_t axis, double value){
        int index = static_cast<int>(axis);
        if (degree.raw & (1 << index))
            Translation[index] += value;
    };
    void Rotate(angles_t angle, double value){
        int index = static_cast<int>(angle);
        if (degree.raw & (8 << index))
            Rotation[index] += value;
    }

    void Set(axis_t axis, double value) {
        int index = static_cast<int>(axis);
        if (degree.raw & (1 << index))
            Translation[index] = value;
    };
    void Set(angles_t angle, double value) {
        int index = static_cast<int>(angle);
        if (degree.raw & (8 << index))
            Rotation[index] = value;
    }

    void SetTranslation (const vertex& v) {
        if (degree.x)
            Translation.x = v.x;
        if (degree.y)
            Translation.y = v.y;
        if (degree.z)
            Translation.z = v.z;
    };
    void SetRotation(const vertex& v) {
        if (degree.alpha)
            Rotation.x = v.x;
        if (degree.beta)
            Rotation.y = v.y;
        if (degree.gamma)
            Rotation.z = v.z;
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
    const std::string Name;

    DependencyNode(std::string Name) : Name(Name) {}

    void LinkTo(DependencyNode* node) {
        if (parent)
            parent->children.remove(this);
        parent = node;
        if(parent)
            parent->children.push_back(this);
    }
    void MakeIndependent() { LinkTo(nullptr); }

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

    DependencyNode* getParent() const { return parent; }
    const std::list<DependencyNode*>& getChildren() const { return children; }

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
    DrawableModel(std::string&& Path) : DependencyNode(Path.substr(Path.find_last_of("\\") + 1)), Path(Path) {}

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