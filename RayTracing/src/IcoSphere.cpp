#define G_RATIO 1.6180339887498948482

#include "IcoSphere.h"
#include "Triangle.h"

/// <summary>
/// Intialization function for an Icosphere object.
/// </summary>
/// <param name="splits">The number of times to tessellate the icosahedron</param>
IcoSphere::IcoSphere(int splits) : m_splits(splits)
{
}

IcoSphere::~IcoSphere()
{
}

/// <summary>
/// Function to generate the vertices.
/// </summary>
/// <param name="indices">The vector into which the function pushes the indices</param>
/// <param name="points">The vector into which the function pushes the vertices</param>
void IcoSphere::generateSphere(std::vector<int>& indices, std::vector<float>& points)
{
    Triangle triangle;
    triangle.update({ 1, G_RATIO, 0 }, { -1, G_RATIO, 0 }, { 0, 1, G_RATIO });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 1, G_RATIO, 0 }, { -1, G_RATIO, 0 }, { 0, 1, -G_RATIO });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 1, -G_RATIO, 0 }, { -1, -G_RATIO, 0 }, { 0, -1, G_RATIO });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 1, -G_RATIO, 0 }, { -1, -G_RATIO, 0 }, { 0, -1, -G_RATIO });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 0, 1, G_RATIO }, { G_RATIO, 0, 1 }, { 1, G_RATIO, 0 });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 0, 1, G_RATIO }, { -G_RATIO, 0, 1 }, { -1, G_RATIO, 0 });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 0, -1, G_RATIO }, { G_RATIO, 0, 1 }, { 1, -G_RATIO, 0 });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 0, -1, G_RATIO }, { -G_RATIO, 0, 1 }, { -1, -G_RATIO, 0 });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 0, 1, -G_RATIO }, { G_RATIO, 0, -1 }, { 1, G_RATIO, 0 });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 0, 1, -G_RATIO }, { -G_RATIO, 0, -1 }, { -1, G_RATIO, 0 });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 0, -1, -G_RATIO }, { G_RATIO, 0, -1 }, { 1, -G_RATIO, 0 });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 0, -1, -G_RATIO }, { -G_RATIO, 0, -1 }, { -1, -G_RATIO, 0 });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 0, 1, G_RATIO }, { G_RATIO, 0, 1 }, { 0, -1, G_RATIO });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 0, 1, G_RATIO }, { -G_RATIO, 0, 1 }, { 0, -1, G_RATIO });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 0, 1, -G_RATIO }, { G_RATIO, 0, -1 }, { 0, -1, -G_RATIO });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 0, 1, -G_RATIO }, { -G_RATIO, 0, -1 }, { 0, -1, -G_RATIO });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ -1, G_RATIO, 0 }, { -G_RATIO, 0, 1 }, { -G_RATIO, 0, -1 });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ -1, -G_RATIO, 0 }, { -G_RATIO, 0, 1 }, { -G_RATIO, 0, -1 });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 1, G_RATIO, 0 }, { G_RATIO, 0, 1 }, { G_RATIO, 0, -1 });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
    triangle.update({ 1, -G_RATIO, 0 }, { G_RATIO, 0, 1 }, { G_RATIO, 0, -1 });
    triangle.tesselate(indices, points, points.size() / 3, m_splits);
}

void IcoSphere::setSplits(int splits)
{
    m_splits = splits;
}
