#include "Graphics/Shader.hpp"

#include <Gl.h>
#include <filesystem>
#include <Core/Core.hpp>

#include "debug/Instrumentation.hpp"

namespace ant
{
	Shader::Shader()
	{
		m_glProgram = GL_INVALID_INDEX;
		m_glProgram = glCreateProgram();

		CORE_ASSERT(m_glProgram != GL_INVALID_INDEX, "Failed do create glProgram (shader)");
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_glProgram);
	}

	void Shader::LoadFromFile(const std::string &filePath)
	{
		CORE_PROFILE_FUNC();

		CORE_ASSERT(
			std::filesystem::exists(filePath), "Cannot find shader file! " + filePath);

		std::ifstream file(filePath);
		std::string line;

		enum : uint8_t
		{
			vertex = 0,
			fragment = 1
		};

		uint8_t type = vertex;

		std::string *shaderSrcPtr[2] = {
			&m_source.vertex,
			&m_source.fragment};

		while (std::getline(file, line))
		{
			if (!line.compare("#vertexShader"))
			{
				type = vertex;
				continue;
			}

			if (!line.compare("#fragmentShader"))
			{
				type = fragment;
				continue;
			}

			line.push_back('\n');

			shaderSrcPtr[type]->append(line);
		}
	}

	void Shader::Init()
	{
		CORE_ASSERT(m_source.vertex.length(), "There has to be a vertex shader source!");
		CORE_ASSERT(m_source.fragment.length(), "There has to be a fragment shader source!");

		unsigned int vertexShader = CompileShader(m_source.vertex, GL_VERTEX_SHADER);
		unsigned int fragmentShader = CompileShader(m_source.fragment, GL_FRAGMENT_SHADER);

		glAttachShader(m_glProgram, vertexShader);
		glAttachShader(m_glProgram, fragmentShader);

		glLinkProgram(m_glProgram);
		glValidateProgram(m_glProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void Shader::Init(const std::string &vertexsrc, const std::string &fragmentsrc)
	{
		m_source.vertex = vertexsrc;
		m_source.fragment = fragmentsrc;
		Init();
	}

	void Shader::Bind()
	{
		glUseProgram(m_glProgram);
	}

	void Shader::UnBind()
	{
		glUseProgram(0);
	}

	int Shader::CompileShader(const std::string &source, uint32_t type)
	{
		CORE_PROFILE_FUNC();
		uint32_t id = glCreateShader(type);
		const char *src = source.c_str();

		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int res;
		glGetShaderiv(id, GL_COMPILE_STATUS, &res);

		if (res == GL_FALSE)
		{
			std::stringstream ss;
			int len;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
			char *mes = (char *)alloca(len);
			glGetShaderInfoLog(id, len, &len, mes);
			ss << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader compilation failed! " << mes;
			CORE_ASSERT(false, ss.str());
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	std::filesystem::path Shader::EngineShaderDir()
	{
		static auto value = std::filesystem::current_path().parent_path().append("Engine/shader");
		return value;
	}

}