#include "ModelLoader.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

/// <summary>
/// Function to load the model
/// </summary>
/// <param name="filepath">The path of the .obj file to be loaded</param>
/// <param name="indices">The vector into which the function pushes the indices</param>
/// <param name="points">The vector into which the function pushes the vertices</param>
void ModelLoader::loadModel(const std::string& filepath, std::vector<int>& indices, std::vector<float>& points)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[4];
    std::string number;
    std::istringstream iss;
    while (getline(stream, line))
    {
        if (line.find("v ") == 0)
        {
            iss = std::istringstream(line);
            iss >> number;
            while (iss >> number)
            {
                points.push_back(atof(number.c_str()));

            }
        }
        else if (line.find("f ") == 0)
        {
            iss = std::istringstream(line);
            iss >> number;
            while (iss >> number)
            {
                indices.push_back(atoi(number.c_str()) - 1);
            }
        }
    }
}
