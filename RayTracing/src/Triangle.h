#pragma once
#include "Vec.h"
#include <vector>

class Triangle
{
private:
	Vec vec1;
	Vec vec2;
	Vec vec3;
public:
	Triangle(Vec vec1, Vec vec2, Vec vec3);
	Triangle();
	void update(Vec vec1, Vec vec2, Vec vec3);
	void tesselate(std::vector<int>& indices, std::vector<float>& points, int offset, int splits);
};