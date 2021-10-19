#include "Pch.h"
#include "Graphics/Shader.hpp"

#include <Gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

namespace ant
{
	Uniform::DataType Uniform::GetDataTypeEnum(const std::string &name)
	{
		if (name == "float")
			return DataType::vec1f;
		if (name == "vec2")
			return DataType::vec2f;
		if (name == "vec3")
			return DataType::vec3f;
		if (name == "vec4")
			return DataType::vec4f;
		if (name == "mat2")
			return DataType::mat2f;
		if (name == "mat3")
			return DataType::mat3f;
		if (name == "mat4")
			return DataType::mat4f;
		if (name == "int")
			return DataType::ivec1;
		if (name == "ivec2")
			return DataType::ivec2;
		if (name == "ivec3")
			return DataType::ivec3;
		if (name == "ivec4")
			return DataType::ivec4;
		if (name == "sampler2D")
			return DataType::ivec1;
		return DataType::incorrect;
	}

	void Uniform::operator=(float data)
	{
		VerifyDataType(DataType::vec1f);
		glUniform1f(m_glUniformId, data);
	}

	void Uniform::operator=(glm::vec2 vec)
	{
		VerifyDataType(DataType::vec2f);
		glUniform2f(m_glUniformId, vec.x, vec.y);
	}

	void Uniform::operator=(glm::vec3 vec)
	{
		VerifyDataType(DataType::vec3f);
		glUniform3f(m_glUniformId, vec.x, vec.y, vec.z);
	}

	void Uniform::operator=(glm::vec4 vec)
	{
		VerifyDataType(DataType::vec4f);
		glUniform4f(m_glUniformId, vec.x, vec.y, vec.z, vec.w);
	}

	void Uniform::operator=(glm::mat2 mat)
	{
		VerifyDataType(DataType::mat2f);
		glUniformMatrix2fv(m_glUniformId, 1, GL_FALSE, glm::value_ptr(mat));
	}

	void Uniform::operator=(glm::mat3 mat)
	{
		VerifyDataType(DataType::mat3f);
		glUniformMatrix3fv(m_glUniformId, 1, GL_FALSE, glm::value_ptr(mat));
	}

	void Uniform::operator=(glm::mat4 mat)
	{
		VerifyDataType(DataType::mat4f);
		glUniformMatrix4fv(m_glUniformId, 1, GL_FALSE, glm::value_ptr(mat));
	}

	void Uniform::operator=(int data)
	{
		VerifyDataType(DataType::ivec1);
		glUniform1i(m_glUniformId, data);
	}

	void Uniform::operator=(glm::ivec2 vec)
	{
		VerifyDataType(DataType::ivec2);
		glUniform2i(m_glUniformId, vec.x, vec.y);
	}

	void Uniform::operator=(glm::ivec3 vec)
	{
		VerifyDataType(DataType::ivec3);
		glUniform3i(m_glUniformId, vec.x, vec.y, vec.z);
	}

	void Uniform::operator=(glm::ivec4 vec)
	{
		VerifyDataType(DataType::ivec4);
		glUniform4i(m_glUniformId, vec.x, vec.y, vec.z, vec.w);
	}

	void Uniform::UploadArray(int *data, size_t size)
	{
		VerifyDataType(DataType::ivec1);
		glUniform1iv(m_glUniformId, size, data);
	}

	void Uniform::SetUniformId(const std::string &name, uint32_t glShaderId)
	{
		if (m_glUniformId == -1)
			m_glUniformId = glGetUniformLocation(glShaderId, name.c_str());
	}

	void Uniform::VerifyDataType(DataType type)
	{
		CORE_ASSERT(type == m_allowedDataType, "Incorrect type of uniform data provided!");
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_shaderId);
	}

	void Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
	{
		CORE_PROFILE_FUNC();
		m_shaderId = glCreateProgram();
		uint32_t vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
		uint32_t fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

		glAttachShader(m_shaderId, vs);
		glAttachShader(m_shaderId, fs);

		glLinkProgram(m_shaderId);
		glValidateProgram(m_shaderId);

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	void Shader::LoadFromFile(const std::string &filePath)
	{
		CORE_PROFILE_FUNC();

		CORE_ASSERT(
        std::filesystem::exists(filePath),"Cannot find shader file! " + filePath);

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

			if (std::regex_match(line, std::regex("(.*)(uniform)(.*)(\n)+(.*)")))
			{
				auto [unName, unType] = ParseUniformInfo(line);
				m_uniforms[unName].SetAllowedDataType(unType);
			}
		}
	}

	std::pair<std::string, Uniform::DataType> Shader::ParseUniformInfo(std::string &line) //? pay attention to the reference
	{
		std::regex regex("([a-zA-Z0-9_]+)");
		std::smatch found;
		Uniform::DataType type;
		std::string name;

		for (size_t j = 0; j < 3; j++)
		{
			if (std::regex_search(line, found, regex))
			{
				if (j == 1)
					type = Uniform::GetDataTypeEnum(found[0]);
				if (j == 2)
					name = found[0];
				line = found.suffix();
			}
		}

		return std::pair(name, type);
	}

	void Shader::BindShader()
	{
		glUseProgram(m_shaderId);
	}

	Uniform::~Uniform()
	{
		Material::BufferType *ptr = (Material::BufferType *)m_buffer;
		delete ptr;
	}

	Uniform &Shader::SetUniform(const std::string &name)
	{
		CORE_PROFILE_FUNC();
		auto &un = m_uniforms[name];
		un.SetUniformId(name, m_shaderId);
		return un;
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

	void Material::Use()
	{
		CORE_PROFILE_FUNC();
		m_shader->BindShader();

		for (auto &it : m_shader->m_uniforms)
		{
			const std::string &name = it.first;
			Uniform &ref = it.second;
			BufferType &buffer = *(BufferType *)ref.m_buffer;
			auto type = ref.GetAllowedDataType();
			if (ref.m_buffer)
			{
				switch (type)
				{
				case Uniform::DataType::vec1f:
					m_shader->SetUniform(name) = buffer.vec1f;
					break;
				case Uniform::DataType::vec2f:
					m_shader->SetUniform(name) = buffer.vec2f;
					break;
				case Uniform::DataType::vec3f:
					m_shader->SetUniform(name) = buffer.vec3f;
					break;
				case Uniform::DataType::vec4f:
					m_shader->SetUniform(name) = buffer.vec4f;
					break;
				case Uniform::DataType::mat2f:
					m_shader->SetUniform(name) = buffer.mat2f;
					break;
				case Uniform::DataType::mat3f:
					m_shader->SetUniform(name) = buffer.mat3f;
					break;
				case Uniform::DataType::mat4f:
					m_shader->SetUniform(name) = buffer.mat4f;
					break;
				case Uniform::DataType::ivec1:
					m_shader->SetUniform(name) = buffer.ivec1;
					break;
				case Uniform::DataType::ivec2:
					m_shader->SetUniform(name) = buffer.ivec2;
					break;
				case Uniform::DataType::ivec3:
					m_shader->SetUniform(name) = buffer.ivec3;
					break;
				case Uniform::DataType::ivec4:
					m_shader->SetUniform(name) = buffer.ivec4;
					break;

				default:
					break;
				}
			}
		}
	}

	void Material::SetBuffer(BufferType &buffer, int val)
	{
		buffer.ivec1 = val;
	}

	void Material::SetBuffer(BufferType &buffer, glm::ivec2 val)
	{
		buffer.ivec2 = val;
	}

	void Material::SetBuffer(BufferType &buffer, glm::ivec3 val)
	{
		buffer.ivec3 = val;
	}

	void Material::SetBuffer(BufferType &buffer, glm::ivec4 val)
	{
		buffer.ivec4 = val;
	}

	void Material::SetBuffer(BufferType &buffer, float val)
	{
		buffer.vec1f = val;
	}

	void Material::SetBuffer(BufferType &buffer, glm::vec2 val)
	{
		buffer.vec2f = val;
	}

	void Material::SetBuffer(BufferType &buffer, glm::vec3 val)
	{
		buffer.vec3f = val;
	}

	void Material::SetBuffer(BufferType &buffer, glm::vec4 val)
	{
		buffer.vec4f = val;
	}

	void Material::SetBuffer(BufferType &buffer, glm::mat2 val)
	{
		buffer.mat2f = val;
	}

	void Material::SetBuffer(BufferType &buffer, glm::mat3 val)
	{
		buffer.mat3f = val;
	}

	void Material::SetBuffer(BufferType &buffer, glm::mat4 val)
	{
		buffer.mat4f = val;
	}

} // namespace ant