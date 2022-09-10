#pragma once
#include <string>
#include <gl/GL.h>

struct vertex
{
    float x;
    float y;
    float z;

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
    vertex() : x(0), y(0), z(0) {}
    vertex(float single) : x(single), y(single), z(single) {}
    vertex(float x, float y, float z) : x(x), y(y), z(z) {}
};

class BaseModel
{
protected:
    const std::string Path;
public:
    const std::string Name;
    
    BaseModel() {}
    BaseModel(const char path[])
        : Path(std::string(path)), Name(Path.substr(Path.find_last_of("\\") + 1))
    {}

    vertex Translation;
    vertex Rotation;
    float Scale = 1;

    BaseModel* Leaf = nullptr;
    BaseModel* Host = nullptr;
    virtual void LinkTo(BaseModel* Host) final { this->Host = Host; Host->Leaf = this; }

    bool Active = false;

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
