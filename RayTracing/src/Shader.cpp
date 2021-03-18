#include <glad/glad.h>
#include "Shader.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath) : m_filePath(filepath), m_rendererID(0)
{
    ShaderProgramSource source = parseShader(filepath);
    m_rendererID = createShader(source.vertex_source, source.fragment_source);    
}

Shader::~Shader()
{
    glDeleteProgram(m_rendererID);
}

/// Parses the shaders from the file path for use in the shader program.
/// 
/// @param filepath The path of the shader file we want to parse.
ShaderProgramSource Shader::parseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

/// Compiles a shader component and returns its id.
/// 
/// @param type The type of shader provided.
/// @param source The contents of the given shader.
unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

/// @static Creates a shader for a shader program.
/// 
/// This function creates and compiles vertex shaders and fragment shaders together into a shader program from a given
/// filepath. The two seperate shaders are then deleted.
/// @returns The resultant the program ID of the created shader program.
/// @param vertex_shader The source of the vertex shader.
/// @param fragment_shader The source of the fragment shader.
unsigned int Shader::createShader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::bind() const
{
    glUseProgram(m_rendererID);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::setUniform4f(const std::string& name, float v1, float v2, float v3, float v4)
{
    glUniform4f(getUniformLocation(name), v1, v2, v3, v4);
}

void Shader::setUniform3f(const std::string& name, float v1, float v2, float v3)
{
    glUniform3f(getUniformLocation(name), v1, v2, v3);
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

unsigned int Shader::getUniformLocation(const std::string& name)
{
    if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
        return m_uniform_location_cache[name];
    int location = glGetUniformLocation(m_rendererID, name.c_str());
    if (location == -1)
        std::cout << "Warning: Uniform '" << name << "' doesn't exist" << std::endl;

    m_uniform_location_cache[name] = location;
    return location;
}
