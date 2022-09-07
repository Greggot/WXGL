#pragma once
#pragma warning(disable : 4996)

#include <list>

#include <cstdio>
#include <string>
#include <vector>

#include <wx/glcanvas.h>
#include <gl/GLU.h>
#include <gl/GL.h>

#include "BaseModel.hpp"

struct poly
{
    int begin;
    int mid;
    int end;
};

typedef vertex color;

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