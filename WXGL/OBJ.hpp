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
    
    Model* Host = nullptr;
    Model* Leaf = nullptr;
public:
    bool Active = false;

    Model() { }
    Model(const char* FilePath);

    void Draw() const;
    vertex* getTransformVector() { return &Translation; }
    vertex* getRotationVector() { return &Rotation; }

    void LinkTo(Model* Host) { this->Host = Host; Host->setLeaf(this); }
    void setLeaf(Model* Leaf) { this->Leaf = Leaf; }
    Model* getHost() const { return Host; }
    Model* getLeaf() const { return Leaf; }
};

}