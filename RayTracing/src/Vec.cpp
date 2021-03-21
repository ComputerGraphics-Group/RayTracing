#include "Vec.h"
Vec Vec::normalize(float length)
{
    float dx = x;
    float dy = y;
    float dz = z;
    float factor = length / getLength();
    dx = dx * factor;
    dy = dy * factor;
    dz = dz * factor;
    return { dx, dy, dz };
}

void Vec::pushIntoVector(std::vector<float>& points)
{
    points.push_back(x);
    points.push_back(y);
    points.push_back(z);
}
