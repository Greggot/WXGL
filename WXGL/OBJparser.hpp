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
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct OBJmodel
{
	std::vector<point> Points;
	std::list<std::list<poly>> Polygons;
    bool Active;
    color Color;

    point Translation;
    point Rotation;
};

OBJmodel ParseFile(const char* path);