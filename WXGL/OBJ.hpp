#pragma once
#pragma warning(disable : 4996)

#include <list>

#include <cstdio>
#include <string>
#include <vector>

#include <wx/glcanvas.h>
#include <gl/GLU.h>
#include <gl/GL.h>

struct vertex
{
    float x;
    float y;
    float z;

    vertex& operator+=(const vertex& rhs) 
    {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }
    vertex& operator-=(const vertex& rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        return *this;
    }
};
struct poly
{
    int begin;
    int mid;
    int end;
};
struct color
{
    float r;
    float g;
    float b;

    color& operator+=(const color& rhs)
    {
        this->r += rhs.r;
        this->g += rhs.g;
        this->b += rhs.b;
        return *this;
    }
    color& operator-=(const color& rhs)
    {
        this->r -= rhs.r;
        this->g -= rhs.g;
        this->b -= rhs.b;
        return *this;
    }
    color(float single)
        : r(single), g(single), b(single)
    {
    }
    color(float r, float g, float b)
        : r(r), g(g), b(b)
    {
    }
};

class BaseModel
{
protected:
    std::string Path;
public:
    std::string Name;

    vertex Translation;
    vertex Rotation;

    BaseModel* Leaf = nullptr;
    BaseModel* Host = nullptr;
    virtual void LinkTo(BaseModel* Host) final { this->Host = Host; Host->Leaf = this; }

    bool Active = false;

    BaseModel() {}
    virtual void Draw() const {};
    virtual void ColorSelectDraw(uint32_t ID) const {};
    
    static uint32_t GetColorSelection(uint32_t x, uint32_t y)
    {
        GLint viewport[4] = { 0 };
        glGetIntegerv(GL_VIEWPORT, viewport);

        GLubyte Color[4] = { 0 };
        glReadPixels(x, viewport[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, Color);

        return (Color[0] << 16) | (Color[1] << 8) | Color[2];
    }
};

namespace OBJ
{

struct Part
{
    std::list<poly> Polygons;
    color Color;
};

class Model : public BaseModel
{
    std::string Path;

    std::vector<vertex> Points;
    std::list<Part> Parts;

    inline void ApplyMovementFromBottomToTop() const;
public:
    Model() { }
    Model(const char* FilePath);

    void Draw() const override;
    void ColorSelectDraw(uint32_t ID) const override;
};

}