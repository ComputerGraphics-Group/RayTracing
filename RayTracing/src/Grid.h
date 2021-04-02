#pragma once
#include <vector>

class Grid
{
private:
	int left;
	int right;
	int front;
	int back;
	int step;
	int z;
public:
	Grid(int left, int right, int front, int back, int step, int z);
	void generateGrid(std::vector<int>& indices, std::vector<float>& points);
};