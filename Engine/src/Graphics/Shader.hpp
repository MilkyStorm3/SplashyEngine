#pragma once
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Graphics/VertexLayout.hpp"
#include "Core/Core.hpp"

namespace df
{
    class Uniform
    {
    public:
        //! ------------------------------------------------------------------------------------------------------------------
        enum class DataType //todo implement all gluniform data types
        {
            incorrect = -1,
            vec1f = 0,
            vec2f,
            vec3f,
            vec4f,
            mat2f,
            mat3f,
            mat4f,
            ivec1,
            ivec2,
            ivec3,
            ivec4
        };

    private:
        static DataType GetDataTypeEnum(const std::string &name);
        //! ------------------------------------------------------------------------------------------------------------------

    public:
        Uniform(DataType allowedType) : m_allowedDataType(allowedType) {}
        Uniform() {}
        ~Uniform();
        friend class Shader;
        friend class Material;
        DataType GetAllowedDataType() const { return m_allowedDataType; }

        void operator=(float data);    // 1f uniform
        void operator=(glm::vec2 vec); // 2f uniform
        void operator=(glm::vec3 vec); // 3f uniform
        void operator=(glm::vec4 vec); // 4f uniform

        void operator=(glm::mat2 mat); // mat2 uniform
        void operator=(glm::mat3 mat); // mat3 uniform
        void operator=(glm::mat4 mat); // mat4 uniform

        void operator=(int data);       // 1i uniform
        void operator=(glm::ivec2 vec); // 2i uniform
        void operator=(glm::ivec3 vec); // 3i uniform
        void operator=(glm::ivec4 vec); // 4i uniform

        void UploadArray(int *data, size_t size); // 1iv uniform
        void SetAllowedDataType(DataType type) const { m_allowedDataType = type; }

    private:
        void SetUniformId(const std::string &name, uint32_t glShaderId);
        void VerifyDataType(DataType type);

    private:
        size_t m_setCount = 1;
        int32_t m_glUniformId = -1;
        mutable DataType m_allowedDataType;
        void *m_buffer = nullptr;
    };

    class Shader
    {
    private:
        friend class Material;

    public:
        static Ref<Shader> Create() { return MakeRef<Shader>(); }
        static Ref<Shader> Create(const std::string &filePath) { return MakeRef<Shader>(filePath); }

        Shader() {}
        Shader(const std::string &filePath) { LoadFromFile(filePath); }
        ~Shader();

        inline void CreateShader() { CreateShader(m_source.vertex, m_source.fragment); }
        void CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
        void LoadFromFile(const std::string &filePath);
        Uniform &SetUniform(const std::string &name);
        void BindShader();

    private:
        static std::pair<std::string, Uniform::DataType> ParseUniformInfo(std::string &line);
        static int CompileShader(const std::string &source, uint32_t type);

    private:
        struct shaderSource
        {
            std::string fragment;
            std::string vertex;
        };

        uint32_t m_shaderId;
        shaderSource m_source;
        std::unordered_map<std::string, Uniform> m_uniforms;
    };

    class Material
    {
    private:
        friend class Uniform;
        union BufferType
        {
            float vec1f;
            glm::vec2 vec2f;
            glm::vec3 vec3f;
            glm::vec4 vec4f;
            int ivec1;
            glm::ivec2 ivec2;
            glm::ivec3 ivec3;
            glm::ivec4 ivec4;
            glm::mat2 mat2f;
            glm::mat3 mat3f;
            glm::mat4 mat4f;
        };

    private:
        static void SetBuffer(BufferType &buffer, int val);
        static void SetBuffer(BufferType &buffer, glm::ivec2 val);
        static void SetBuffer(BufferType &buffer, glm::ivec3 val);
        static void SetBuffer(BufferType &buffer, glm::ivec4 val);
        static void SetBuffer(BufferType &buffer, float val);
        static void SetBuffer(BufferType &buffer, glm::vec2 val);
        static void SetBuffer(BufferType &buffer, glm::vec3 val);
        static void SetBuffer(BufferType &buffer, glm::vec4 val);
        static void SetBuffer(BufferType &buffer, glm::mat2 val);
        static void SetBuffer(BufferType &buffer, glm::mat3 val);
        static void SetBuffer(BufferType &buffer, glm::mat4 val);

    public:
        Material(const Ref<Shader> &shader) : m_shader(shader) {}
        ~Material() {}
        static Ref<Material> Create(const Ref<Shader> &shader) { return MakeRef<Material>(shader); }
        void Use();

        template <class T>
        void Set(const std::string &name, const T &uniformVal)
        {
            Uniform *uniform = &m_shader->m_uniforms[name];
            uniform->m_buffer = new BufferType;
            SetBuffer(*(BufferType *)uniform->m_buffer, uniformVal);
        }
        Ref<Shader> GetShader() { return m_shader; }

    private:
        struct BufferIndex
        {
            std::string *name;
            Uniform *unf;
        };
        Ref<Shader> m_shader;
    };

} // namespace df
