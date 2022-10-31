#include <Model/BaseModel.hpp>

void vertex::draw() const { glVertex3f(x, y, z); }
float vertex::maxvalue() const { return x > y ? (x > z ? x : z) : (y > z ? y : z); }
