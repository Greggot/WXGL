#pragma once
#pragma warning(disable : 4996)

#include <list>

#include <cstdio>
#include <string>
#include <vector>

struct point
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

struct Part
{
    std::list<poly> Polygons;
    color Color;
};

struct OBJmodel
{
	std::vector<point> Points;
	std::list<Part> Parts;
    
    bool Active;

    point Translation;
    point Rotation;
};

OBJmodel ParseFile(const char* path);