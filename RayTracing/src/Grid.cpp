#include "Grid.h"
#include <iostream>

Grid::Grid(int left, int right, int back, int front, int step, int z): left(left), right(right), front(front), back(back), step(step), z(z)
{
}

void Grid::generateGrid(std::vector<int>& indices, std::vector<float>& points)
{
	for (int i = left; i <= right; i += step)
	{
		for (int j = back; j <= front; j += step)
		{
			points.push_back(i);
			points.push_back(z);
			points.push_back(j);
		}
	}

	int vertical = (right - left) / step;
	int horizontal = (front - back) / step;

	std::cout << vertical << " " << horizontal << std::endl;

	for (int i = 0; i < vertical; i++)
	{
		for (int j = 0; j < horizontal; j++)
		{
			indices.push_back(i * (vertical + 1) + j);
			indices.push_back(i * (vertical + 1) + j + 1);
			indices.push_back((i + 1) * (vertical + 1) + j);
		}
	}
}
