#include "Grid.h"
#include <iostream>

/// <summary>
/// Intialization function for a Grid object.
/// </summary>
/// <param name="left">The left boundary</param>
/// <param name="right">The right boundary</param>
/// <param name="back">The back boundary</param>
/// <param name="front">The front boundary</param>
/// <param name="step">The space between two grid lines</param>
/// <param name="y">The y coordinate of the entire grid</param>
Grid::Grid(int left, int right, int back, int front, int step, int y): left(left), right(right), front(front), back(back), step(step), y(y)
{
}

/// <summary>
/// Function to generate the points.
/// </summary>
/// <param name="indices">The vector into which the function pushes the indices</param>
/// <param name="points">The vector into which the function pushes the vertices</param>
void Grid::generateGrid(std::vector<int>& indices, std::vector<float>& points)
{
	for (int i = left; i <= right; i += step)
	{
		for (int j = back; j <= front; j += step)
		{
			points.push_back(i);
			points.push_back(y);
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
