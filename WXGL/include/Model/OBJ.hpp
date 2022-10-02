#pragma once
#pragma warning(disable : 4996)

#include <list>
#include <stdexcept>

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

enum class symbol : char
{
    vertex = 'v',
    polygon = 'f',
    part = 'u',
    material = 'm',

    nameOfMaterial = 'n',
    color = 'K',
};

namespace OBJ
{
    struct Part
    {
        std::list<poly> Polygons;
        color Color;

        Part() : Polygons{}, Color(standardColor) {}
    };

    class Model : public BaseModel
    {
    private:
        std::vector<vertex> Points;
        std::list<Part> Parts;

        inline vertex stringContainerToVertex(const std::vector<std::string>& strings);

        inline void DrawPoly(const poly&) const;
        inline void DrawPolyOutline(const poly&) const;

        color GetColorFromMTLfile(const char* path, const char* base);
        void AddPart(Part&, const char[], const char[]);
    public:
        Model() { }
        Model(const char* FilePath);

        void Draw() const override;
        void DrawSelectionMode(uint32_t ID) const override;
        void ActiveOutlineDraw() const override;

        ~Model() { }
    };
}