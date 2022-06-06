#pragma once
#pragma warning(disable : 4996)

#include <list>

#include <cstdio>
#include <string>
#include <vector>

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

struct Model
{
    std::vector<vertex> Points;
    std::list<Part> Parts;

    bool Active;

    vertex Translation;
    vertex Rotation;
};

Model Parse(const char* FilePath);

}