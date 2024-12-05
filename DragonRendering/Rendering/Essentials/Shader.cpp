#include "Shader.h"
#include <iostream>

namespace DragonRendering
{
    GLuint Shader::GetUniformLocation(const std::string& UniformName)
    {
        auto UniformItr = m_UniformLocationMap.find(UniformName);
        if (UniformItr != m_UniformLocationMap.end())
        {
            return UniformItr->second;
        }

        GLuint Location = glGetUniformLocation(m_ShaderProgramID, UniformName.c_str());
        if (Location == GL_INVALID_INDEX)
        {
            std::cout << "Unirform [" << UniformName << "] not found in the Shader" << std::endl;
            return -1;
        }

        m_UniformLocationMap.emplace(UniformName, Location);

        return Location;
    }

    Shader::Shader(GLuint Program, const std::string& VertexPath, const std::string& FragmentPath) :
        m_ShaderProgramID{ Program }, m_VertexPath{ VertexPath }, m_FragmentPath{ FragmentPath }
    {

    }

    Shader::~Shader()
    {
        if (m_ShaderProgramID > 0)
        {
            glDeleteProgram(m_ShaderProgramID);
        }
    }

    void Shader::SetUniformInt(const std::string& Name, int Value)
    {
        glUniform1i(GetUniformLocation(Name), Value);
    }

    void Shader::SetUniformMat4(const std::string& Name, glm::mat4& Mat)
    {
        glUniformMatrix4fv(GetUniformLocation(Name), 1, GL_FALSE, &Mat[0][0]);
    }

    void Shader::Enable()
    {
        glUseProgram(m_ShaderProgramID);
    }

    void Shader::Disable()
    {
        glUseProgram(0);
    }
}
