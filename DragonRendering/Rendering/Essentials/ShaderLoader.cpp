#include "ShaderLoader.h"
#include <fstream>
#include <iostream>

namespace DragonRendering 
{
    GLuint ShaderLoader::CreateProgram(const std::string& VertexShader, const std::string& FragmetShader)
    {
        const GLuint Program = glCreateProgram();

        const GLuint Vertex = CompileShader(GL_VERTEX_SHADER, VertexShader);
        const GLuint Fragment = CompileShader(GL_FRAGMENT_SHADER, FragmetShader);

        if (Vertex == 0 || Fragment == 0) return 0;

        if (!LinkShaders(Program,Vertex,Fragment))
        {
            std::cout << "Failed to link shaders!" << std::endl;
            return 0;
        }

        return Program;
    }

    GLuint ShaderLoader::CompileShader(GLuint ShaderType, const std::string& FilePath)
    {
        const GLuint  ShaderID = glCreateShader(ShaderType);

        std::ifstream Ifs(FilePath);

        if (Ifs.fail())
        {
            std::cout << "Shader Failed to Open [" << FilePath << "]" << std::endl;
            return 0;
        }

        std::string Contents{ "" };
        std::string Line;

        while (std::getline(Ifs,Line))
        {
            Contents += Line + "\n";
        }

        Ifs.close();

        const char* ContentsPtr = Contents.c_str();
        glShaderSource(ShaderID, 1, &ContentsPtr, nullptr);

        glCompileShader(ShaderID);
        if (!CompileSucess(ShaderID))
        {
            std::cout << "Failed to compile shader [" << FilePath << "]" << std::endl;
            return 0;
        }
        return ShaderID;
    }

    bool ShaderLoader::CompileSucess(GLuint Shader)
    {
        GLint Status;
        glGetShaderiv(Shader, GL_COMPILE_STATUS, &Status);

        if (Status != GL_TRUE)
        {
            GLint MaxLength;
            glGetShaderiv(Shader,GL_INFO_LOG_LENGTH, &MaxLength);
            std::string ErrorLog(MaxLength,' ');
            glGetShaderInfoLog(Shader, MaxLength, &MaxLength, ErrorLog.data());

            std::cout << "Shader Compilation Failed: " << std::string{ErrorLog} << std::endl;

            glDeleteShader(Shader);
            return false;
        }
        return true;
    }

    bool ShaderLoader::IsProgramValid(GLuint Program)
    {
        GLint Status;
        glGetProgramiv(Program, GL_LINK_STATUS, &Status);

        if (Status != GL_TRUE)
        {
            GLint MaxLength;
            glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &MaxLength);
            std::string ErrorLog(MaxLength, ' ');
            glGetProgramInfoLog(Program, MaxLength, &MaxLength, ErrorLog.data());

            std::cout << "Shader Program failed to link: " << std::string{ ErrorLog } << std::endl;
            return false;
        }
        return true;
    }

    bool ShaderLoader::LinkShaders(GLuint Program, GLuint VertexShader, GLuint FragmentShader)
    {
        glAttachShader(Program, VertexShader);
        glAttachShader(Program, FragmentShader);

        glLinkProgram(Program);
        
        if (!IsProgramValid(Program))
        {
            glDeleteProgram(Program);

            glDeleteShader(VertexShader);
            glDeleteShader(FragmentShader);

            return false;
        }

        glDetachShader(Program, VertexShader);
        glDeleteShader(VertexShader);

        glDetachShader(Program, FragmentShader);
        glDeleteShader(FragmentShader);

        return true;
    }

    std::shared_ptr<Shader> ShaderLoader::Create(const std::string& VertexShaderPath, const std::string& FragmentShaderPath)
    {
        GLuint Program = CreateProgram(VertexShaderPath, FragmentShaderPath);

        if (Program != GL_TRUE)
        {
            return nullptr;
        }

        return std::make_shared<Shader>(Program, VertexShaderPath, FragmentShaderPath);
    }
}
