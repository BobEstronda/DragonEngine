#pragma once

#include "Shader.h"
#include <memory>

namespace DragonRendering
{
	class ShaderLoader
	{
	private:
		static GLuint CreateProgram(const std::string& VertexShader, const std::string& FragmetShader);
		static GLuint CompileShader(GLuint ShaderType, const std::string& FilePath);
		static bool CompileSucess(GLuint Shader);
		static bool IsProgramValid(GLuint Program);
		static bool LinkShaders(GLuint Program, GLuint VertexShader, GLuint FragmentShader);


	public:

		ShaderLoader() = delete;
		static std::shared_ptr<Shader> Create(const std::string& VertexShaderPath, const std::string& FragmentShaderPath);

	};
}

