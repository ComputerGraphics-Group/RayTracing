#pragma once
#include <vector>
#include "Vec.h"

class Cone
{
private:
	float m_height;
	float m_radius;
	int m_splits;
public:
	Cone(float height, float radius, int splits);
	~Cone();
	void setParams(float height, float radius, int splits);
	void generate(std::vector<int>& indices, std::vector<float>& points);
};