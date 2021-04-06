#include "OctaSphere.h"
#include "Triangle.h"

/// <summary>
/// Intialization function for an Octasphere object.
/// </summary>
/// <param name="radius">The radius of the sphere</param>
/// <param name="splits">The number of times to tessellate the octahedron</param>
OctaSphere::OctaSphere(int radius, int splits) : m_radius(radius), m_splits(splits)
{
}

OctaSphere::~OctaSphere()
{
}

/// <summary>
/// Function to generate the vertices.
/// </summary>
/// <param name="indices">The vector into which the function pushes the indices</param>
/// <param name="points">The vector into which the function pushes the vertices</param>
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
/// <summary>
/// Function to set the splits.
/// </summary>
/// <param name="splits">The number of splits</param>
void OctaSphere::setSplits(int splits)
{
    m_splits = splits;
}
