#pragma once
struct vertex
{
    float x;
    float y;
    float z;

    vertex() : x(0), y(0), z(0) {}
    vertex(float single) : x(single), y(single), z(single) {}
    vertex(float x, float y, float z) : x(x), y(y), z(z) {}

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

    void draw() const;
    float maxvalue() const;
};