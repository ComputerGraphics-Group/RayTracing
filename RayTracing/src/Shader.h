#pragma once
#include <string>
#include <unordered_map>
#include "external/glm/glm.hpp"
#include "external/glm/gtc/type_ptr.hpp"

/// Holds the source files for the shaders.
/// 
/// This structure contains two strings containing the file source for the vertex and fragment shaders.
struct ShaderProgramSource
{
	/// Source file of the vertex shader.
	std::string vertex_source;
	/// Source file of the fragment shader.
	std::string fragment_source;
};

class Shader
{
private:
	std::string m_filePath;
	unsigned int m_rendererID;
	std::unordered_map<std::string, int> m_uniform_location_cache;

public:
	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	//Set uniforms
	void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
	void setUniform3f(const std::string& name, float v1, float v2, float v3);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	ShaderProgramSource parseShader(const std::string& filepath);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(const std::string& vertex_shader, const std::string& fragment_shader);
	unsigned int getUniformLocation(const std::string& name);
};