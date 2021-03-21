#pragma once
#include <cmath>
#include <vector>

struct Vec
{
    float x;
    float y;
    float z;
    double getLength() { return sqrt(x * x + y * y + z * z); };
    Vec addVec(Vec vec) { return { x + vec.x, y + vec.y, z + vec.z }; };
    Vec subtractVec(Vec vec) { return { x - vec.x, y - vec.y, z - vec.z }; };
    Vec scale(float scale) { return { x * scale, y * scale, z * scale }; };
    Vec normalize(float length);
    void pushIntoVector(std::vector<float>& points);
};