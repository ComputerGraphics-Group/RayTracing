#pragma once
#include <vector>

class IcoSphere
{
private:
	int m_splits;

public:
	IcoSphere(int splits);
	~IcoSphere();
	void generateSphere(std::vector<int>& indices, std::vector<float>& points);
	void setSplits(int splits);
};