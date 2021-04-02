#include <string>
#include <vector>
class ModelLoader
{
public:
	void loadModel(const std::string& filepath, std::vector<int>& indices, std::vector<float>& points);
};