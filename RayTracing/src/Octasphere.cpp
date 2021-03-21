#include "OctaSphere.h"
#include "Triangle.h"

OctaSphere::OctaSphere(int splits) : m_splits(splits)
{
}

OctaSphere::~OctaSphere()
{
}

void OctaSphere::generateSphere(std::vector<int>& indices, std::vector<float>& points)
{
    Triangle triangle;
    triangle.update({ 10, 0, 0 }, { 0, 10, 0 }, { 0, 0, 10 });
    triangle.tesselate(indices, points, points.size() / 6, m_splits);
    triangle.update({ -10, 0, 0 }, { 0, 10, 0 }, { 0, 0, 10 });
    triangle.tesselate(indices, points, points.size() / 6, m_splits);
    triangle.update({ 10, 0, 0 }, { 0, 10, 0 }, { 0, 0, -10 });
    triangle.tesselate(indices, points, points.size() / 6, m_splits);
    triangle.update({ -10, 0, 0 }, { 0, 10, 0 }, { 0, 0, -10 });
    triangle.tesselate(indices, points, points.size() / 6, m_splits);
    triangle.update({ 10, 0, 0 }, { 0, -10, 0 }, { 0, 0, 10 });
    triangle.tesselate(indices, points, points.size() / 6, m_splits);
    triangle.update({ -10, 0, 0 }, { 0, -10, 0 }, { 0, 0, 10 });
    triangle.tesselate(indices, points, points.size() / 6, m_splits);
    triangle.update({ 10, 0, 0 }, { 0, -10, 0 }, { 0, 0, -10 });
    triangle.tesselate(indices, points, points.size() / 6, m_splits);
    triangle.update({ -10, 0, 0 }, { 0, -10, 0 }, { 0, 0, -10 });
    triangle.tesselate(indices, points, points.size() / 6, m_splits);
}

void OctaSphere::setSplits(int splits)
{
    m_splits = splits;
}
