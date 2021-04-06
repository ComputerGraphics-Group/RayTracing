#include "Cone.h"
#include <iostream>

/// <summary>
/// Intialization function for a cone object.
/// </summary>
/// <param name="height">The height of the cone</param>
/// <param name="radius">The base radius</param>
/// <param name="splits">The number of times to tessellate the faces of the square pyramid</param>
Cone::Cone(float height, float radius, int splits) : m_height(height), m_radius(radius), m_splits(splits)
{
}

Cone::~Cone()
{
}

/// <summary>
/// Function to set the height and radius.
/// </summary>
/// <param name="height">The height of the cone</param>
/// <param name="radius">The base radius</param>
/// <param name="splits">The number of times to tessellate the faces of the square pyramid</param>
void Cone::setParams(float height, float radius, int splits)
{
	m_height = height;
	m_radius = radius;
	m_splits = splits;
}

/// <summary>
/// Function to generate the vertices.
/// </summary>
/// <param name="indices">The vector into which the function pushes the indices</param>
/// <param name="points">The vector into which the function pushes the vertices</param>
void Cone::generate(std::vector<int>& indices, std::vector<float>& points)
{
	int divisions = pow(2, m_splits);
	Vec apex = { 0, m_height, 0 };
	Vec base = { 0, 0, 0 };
	Vec directrix = { m_radius, 0, 0 };
	Vec incd = { 0, 0, -m_radius };
	incd = incd.subtractVec(directrix).scale((float)1/(float)divisions);
	
	apex.pushIntoVector(points);
	base.pushIntoVector(points);

	for (int i = 0; i < divisions; ++i)
	{
		directrix.normalize(m_radius).pushIntoVector(points);
		directrix = directrix.addVec(incd);
	}
	incd = { incd.x, incd.y, -incd.z };
	for (int i = 0; i < divisions; ++i)
	{
		directrix.normalize(m_radius).pushIntoVector(points);
		directrix = directrix.addVec(incd);
	}
	incd = { -incd.x, incd.y, incd.z };
	for (int i = 0; i < divisions; ++i)
	{
		directrix.normalize(m_radius).pushIntoVector(points);
		directrix = directrix.addVec(incd);
	}
	incd = { incd.x, incd.y, -incd.z };
	for (int i = 0; i < divisions; ++i)
	{
		directrix.normalize(m_radius).pushIntoVector(points);
		directrix = directrix.addVec(incd);
	}
	int i;
	for (i = 0; i < 4 * (divisions) - 1; ++i)
	{
		indices.push_back(0);
		indices.push_back(i+2);
		indices.push_back(i+3);

		indices.push_back(1);
		indices.push_back(i + 2);
		indices.push_back(i + 3);
	}
	indices.push_back(0);
	indices.push_back(i + 2);
	indices.push_back(2);

	indices.push_back(1);
	indices.push_back(i + 2);
	indices.push_back(2);
}
