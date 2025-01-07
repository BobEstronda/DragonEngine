#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace DragonRendering
{
	class Shader
	{

	private:

		GLuint m_ShaderProgramID;
		std::string m_VertexPath, m_FragmentPath;

		std::unordered_map<std::string, GLuint> m_UniformLocationMap;

		GLuint GetUniformLocation(const std::string& UniformName);

	public:
		Shader() = default;
		Shader(GLuint Program, const std::string& VertexPath, const std::string& FragmentPath);
		~Shader();

		// TODO: add more Setters as needed

		void SetUniformInt(const std::string& Name, int Value);

		void SetUniformMat4(const std::string& Name, glm::mat4& Mat);

		// TODO: add the Getters as needed

		void Enable();
		void Disable();

		inline const GLuint GetShaderProgramID() const { return m_ShaderProgramID; }

		

	};
}