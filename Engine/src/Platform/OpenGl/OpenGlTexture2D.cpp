#include "OpenGlTexture2D.hpp"
#include "stb_image.h"
#include <numeric>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace ant::OpenGl
{
    namespace Utils
    {

        static GLenum ParameterToGLenum(TextureParam parameter)
        {

            switch (parameter)
            {
            case TextureParam::MagnificationFilter:
                return GL_TEXTURE_MAG_FILTER;
            case TextureParam::MinificationFilter:
                return GL_TEXTURE_MIN_FILTER;
            case TextureParam::XWrap:
                return GL_TEXTURE_WRAP_S;
            case TextureParam::YWrap:
                return GL_TEXTURE_WRAP_T;
            case TextureParam::ZWrap:
                return GL_TEXTURE_WRAP_R;
            }

            CORE_ASSERT(false, "Invalid enum");
            return 0;
        }

        static GLint ParaterterValueToGLint(TextureParamValue value)
        {

            switch (value)
            {
            case TextureParamValue::Linear:
                return GL_LINEAR;
            case TextureParamValue::LinearMipmapLinear:
                return GL_LINEAR_MIPMAP_LINEAR;
            case TextureParamValue::LinearMipmapNearest:
                return GL_LINEAR_MIPMAP_NEAREST;
            case TextureParamValue::Nearest:
                return GL_NEAREST;
            case TextureParamValue::NearestMipmapLinear:
                return GL_NEAREST_MIPMAP_LINEAR;
            case TextureParamValue::NearestMipmapNearest:
                return GL_NEAREST_MIPMAP_NEAREST;

            case TextureParamValue::ClampToEdge:
                return GL_CLAMP_TO_EDGE;
            case TextureParamValue::ClampToBorder:
                return GL_CLAMP_TO_BORDER;
            case TextureParamValue::MirroredRepeat:
                return GL_MIRRORED_REPEAT;
            case TextureParamValue::Repeat:
                return GL_REPEAT;
            case TextureParamValue::MirrorClampToEdge:
                return GL_MIRROR_CLAMP_TO_EDGE;
            }
            CORE_ASSERT(false, "Invalid enum");
            return 0;
        }

    }

    GlTexture2D::GlTexture2D(const std::filesystem::path &path)
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        namespace fs = std::filesystem;
        CORE_ASSERT(!path.empty() && !fs::is_empty(path), "Invalid texture path or file");
        CORE_ASSERT(path.extension() != ".jpeg", "Jpeg image format is not supported");
        stbi_set_flip_vertically_on_load(true);
        int x, y, channelCount;
        stbi_uc *imageData = nullptr;
        imageData = stbi_load(path.c_str(), &x, &y, &channelCount, STBI_default);
        CORE_ASSERT(imageData != nullptr, "Error while loading texture file");

        int limit = std::numeric_limits<uint16_t>::max();
        CORE_ASSERT(x <= limit && y <= limit, "Texture too big!");
        m_width = x;
        m_height = y;

        GLenum internalFormat;
        switch (channelCount)
        {
        case 3:
            internalFormat = GL_RGB8;
            m_inputFormat = GL_RGB;
            break;

        case 4:
            internalFormat = GL_RGBA8;
            m_inputFormat = GL_RGBA;
            break;

        default:
            CORE_ASSERT(false, "Unsopported or invalid image format");
            break;
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &m_glID);
        glTextureStorage2D(m_glID, 1, internalFormat, m_width, m_height);

        glTextureSubImage2D(m_glID, 0, 0, 0, m_width, m_height, m_inputFormat, GL_UNSIGNED_BYTE, imageData);
        glGenerateTextureMipmap(m_glID);

        stbi_image_free(imageData);
        m_isLoaded = true;
    }

    GlTexture2D::GlTexture2D(uint32_t width, uint32_t height)
        : m_width(width), m_height(height)
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        GLenum internalFormat = GL_RGBA8;
        m_inputFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_glID);
        glTextureStorage2D(m_glID, 1, internalFormat, m_width, m_height);

        glTextureParameteri(m_glID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_glID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_glID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_glID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    GlTexture2D::~GlTexture2D()
    {
        CORE_DETAILED_PROFILE_FUNC();
        glDeleteTextures(1, &m_glID);
    }

    uint32_t GlTexture2D::GetSizeX() const
    {
        return m_width;
    }

    uint32_t GlTexture2D::GetSizeY() const
    {
        return m_height;
    }

    uint32_t GlTexture2D::GetRendererId() const
    {
        return m_glID;
    }

    void GlTexture2D::Bind(uint32_t slot) const
    {
        CORE_DETAILED_PROFILE_FUNC();
        CORE_ASSERT(m_isLoaded, "Cannot bind unloaded texture");
        glBindTextureUnit(slot, m_glID);
    }

    void GlTexture2D::SetData(void *data, uint32_t size)
    {
        CORE_INTERMEDIATE_PROFILE_FUNC();
        uint16_t cpp = m_inputFormat == GL_RGBA ? 4 : 3;
        CORE_ASSERT(cpp * m_width * m_glID == size, "Data size must cover entire texture");
        glTextureSubImage2D(m_glID, 0, 0, 0, m_width, m_height, m_inputFormat, GL_UNSIGNED_BYTE, data);
    }

    bool GlTexture2D::IsLoaded() const
    {
        return m_isLoaded;
    }

    void GlTexture2D::SetBorderColor(const glm::vec4 &color)
    {
        glTextureParameterfv(m_glID, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color));
    }

    void GlTexture2D::SetLevelOfDetail(float min, float max)
    {
        glTextureParameterf(m_glID, GL_TEXTURE_MIN_LOD, min);
        glTextureParameterf(m_glID, GL_TEXTURE_MAX_LOD, max);
    }

    void GlTexture2D::SetParameter(TextureParam param, TextureParamValue value)
    {
        glTextureParameteri(m_glID, Utils::ParameterToGLenum(param), Utils::ParaterterValueToGLint(value));
    }

}