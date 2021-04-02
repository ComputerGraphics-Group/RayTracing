#include "OctaSphere.h"
#include "Triangle.h"

OctaSphere::OctaSphere(int radius, int splits) : m_radius(radius), m_splits(splits)
{
}

OctaSphere::~OctaSphere()
{
}

void OctaSphere::generateSphere(std::vector<int>& indices, std::vector<float>& points)
{
    Triangle triangle;
    triangle.update({ m_radius, 0, 0 }, { 0, m_radius, 0 }, { 0, 0, m_radius });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ -m_radius, 0, 0 }, { 0, m_radius, 0 }, { 0, 0, m_radius });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ m_radius, 0, 0 }, { 0, m_radius, 0 }, { 0, 0, -m_radius });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ -m_radius, 0, 0 }, { 0, m_radius, 0 }, { 0, 0, -m_radius });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ m_radius, 0, 0 }, { 0, -m_radius, 0 }, { 0, 0, m_radius });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ -m_radius, 0, 0 }, { 0, -m_radius, 0 }, { 0, 0, m_radius });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ m_radius, 0, 0 }, { 0, -m_radius, 0 }, { 0, 0, -m_radius });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ -m_radius, 0, 0 }, { 0, -m_radius, 0 }, { 0, 0, -m_radius });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
}

void OctaSphere::setSplits(int splits)
{
    m_splits = splits;
}
