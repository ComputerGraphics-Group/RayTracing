#include "Cone.h"
#include <iostream>

Cone::Cone(float height, float radius, int splits) : m_height(height), m_radius(radius), m_splits(splits)
{
}

Cone::~Cone()
{
}

void Cone::setParams(float height, float radius, int splits)
{
	m_height = height;
	m_radius = radius;
	m_splits = splits;
}

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
