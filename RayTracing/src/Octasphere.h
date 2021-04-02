#pragma once
#include <vector>
#include "Vec.h"

class OctaSphere
{
private:
	int m_splits;
	float m_radius;

public:
	OctaSphere(int radius, int splits);
	~OctaSphere();
	void generateSphere(std::vector<int>& indices, std::vector<float>& points);
	void setSplits(int splits);
};