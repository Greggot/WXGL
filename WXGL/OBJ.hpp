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
};

namespace OBJ
{

struct Part
{
    std::list<poly> Polygons;
    color Color;
};

class Model
{
    std::vector<vertex> Points;
    std::list<Part> Parts;

    vertex Translation;
    vertex Rotation;
    
    Model* Host;
public:
    bool Active;

    Model() { Active = false; }
    Model(const char* FilePath);

    void Draw() const;
    vertex* getTransformVector() { return &Translation; }
    vertex* getRotationVector() { return &Rotation; }

    void LinkTo(Model* Host) { this->Host = Host; };
};

}