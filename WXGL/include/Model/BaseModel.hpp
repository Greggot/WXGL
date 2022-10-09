#pragma once
#include <string>
#include <list>
#include <algorithm>

#include <windows.h>
#include <gl/GL.h>

#include <vertex.h>

typedef vertex color;
static const color standardColor{ 0.5 };

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
    
    BaseModel() = default;
    BaseModel(const char path[])
        : Path(std::string(path)), Name(Path.substr(Path.find_last_of("\\") + 1))
    {}

    vertex Translation;
    vertex Rotation;
    float Scale = 1;
    bool Active = false;

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
