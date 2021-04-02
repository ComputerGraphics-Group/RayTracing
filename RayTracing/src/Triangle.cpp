#include "Triangle.h"
#include <iostream>

Triangle::Triangle(Vec vec1, Vec vec2, Vec vec3) : vec1(vec1), vec2(vec2), vec3(vec3)
{
}

Triangle::Triangle() : vec1({ 0, 0, 0 }), vec2({ 0, 0, 0 }), vec3({ 0, 0, 0 })
{
}

void Triangle::update(Vec veca, Vec vecb, Vec vecc)
{
    vec1 = veca;
    vec2 = vecb;
    vec3 = vecc;
}

void Triangle::tesselate(std::vector<int>& indices, std::vector<float>& points, int offset, int splits)
{
    int divisions = pow(2, splits);

    Vec veca = vec1;
    Vec vecb = vec1;
    Vec incra = vec2.subtractVec(vec1).scale((float)1 / (float)divisions);
    Vec incrb = vec3.subtractVec(vec1).scale((float)1 / (float)divisions);

    for (int i = 0; i <= divisions; ++i)
    {
        for (int j = i; j <= divisions; ++j)
        {
            vecb.normalize(100).pushIntoVector(points);      //Vertex
//            vecb.normalize(1).pushIntoVector(points);        //Normal
            vecb = vecb.addVec(incrb);
        }
        veca = veca.addVec(incra);
        vecb = veca;
    }

    int a = 0;
    int ia = divisions + 1;
    for (int i = 0; i < divisions; ++i)
    {
        for (int j = 0; j < divisions - i; ++j)
        {
            indices.push_back(a + j + offset);
            indices.push_back(a + j + 1 + offset);
            indices.push_back(a + ia + j + offset);
            if (j < divisions - i - 1)
            {
                indices.push_back(a + j + 1 + offset);
                indices.push_back(a + ia + j + offset);
                indices.push_back(a + ia + j + 1 + offset);
            }
        }
        a += ia;
        ia--;
    }
}
