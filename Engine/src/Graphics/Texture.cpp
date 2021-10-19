#include "Pch.h"

#include "Graphics/Texture.hpp"
#include <stb_image.h>
#include <Gl.h>
#include <filesystem>

namespace df
{

    std::unordered_map<std::string,Ref<Texture>> Texture::s_loadedTextures;

    Ref<Texture> Texture::Create(const std::string &filePath)
    {
        if(s_loadedTextures[filePath]){
            CORE_WARN("Texture asset {0} already loaded - using cached data", filePath);
            return s_loadedTextures[filePath];
        }

        auto ref = MakeRef<Texture>();
        ref->LoadFromFile(filePath);
        s_loadedTextures[filePath] = ref;

        return ref;
    }

    Ref<Texture> Texture::Create(const glm::ivec2 &dimensions, uint32_t channelCount)
    {
        auto ref = MakeRef<Texture>();
        ref->SetFormat(channelCount);
        ref->m_dimensions = dimensions;
        return ref;
    }

    Texture::Texture(bool keepLocalBuffer) : m_keepLocalBuffer(keepLocalBuffer), m_dimensions(0), m_internalFormat(GL_RGBA8), m_subimageFormat(GL_RGBA)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_glId);
    }

    Texture::~Texture()
    {
        if (!m_keepLocalBuffer)
        {
            stbi_image_free(m_rawData);
            m_rawData = nullptr;
        }
        glDeleteTextures(1, &m_glId);

        //! textures loaded one never disappear there is always last reference in s_loadedTextures map

    }

    void Texture::LoadFromFile(const std::string &filePath)
    {
        CORE_ASSERT(
        std::filesystem::exists(filePath),"Cannot find texture file! " + filePath);

        int32_t channelCount = 4;
        m_rawData = stbi_load(filePath.c_str(), &m_dimensions.x, &m_dimensions.y, &channelCount, STBI_default);
        SetFormat(channelCount);
        m_slot = -1;
        m_uploaded = false;
    }

    void Texture::Upload(uint32_t slot)
    {
        if (!m_uploaded)
        {
            glTextureStorage2D(m_glId, 1, m_internalFormat, m_dimensions.x, m_dimensions.y);

            glTextureParameteri(m_glId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_glId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTextureSubImage2D(m_glId, 0, 0, 0, m_dimensions.x, m_dimensions.y, m_subimageFormat, GL_UNSIGNED_BYTE, m_rawData);

            m_uploaded = true;
        }

        if (slot != m_slot)
        {
            m_slot = slot;
            glBindTextureUnit(slot, m_glId);
        }
    }
 
    void Texture::ClearLocalBuffer()
    {
        stbi_image_free(m_rawData);
        m_rawData = nullptr;
    }

    void Texture::SetData(void *data, int32_t size)
    {
        int32_t bpp = m_subimageFormat == GL_RGBA ? 4 : 3;
        CORE_ASSERT(size == m_dimensions.x * m_dimensions.y * bpp, "Incorrect texture data format or size set!");
        stbi_image_free(m_rawData);
        m_rawData = (uchar *)data;
        m_slot = -1;
        m_uploaded = false;
    }

    void Texture::SetFormat(uint32_t channelCount)
    {
        switch (channelCount)
        {
        case 3:
            m_internalFormat = GL_RGB8;
            m_subimageFormat = GL_RGB;
            break;
        case 4:
            m_internalFormat = GL_RGBA8;
            m_subimageFormat = GL_RGBA;
            break;

        default:
            break;
        }
    }

    Ref<SubTexture> SubTexture::Create(Ref<Texture> tex, const glm::vec2 &indecies, const glm::vec2 &tileSize, const glm::vec2 &cellSize)
    {
        auto subTex = MakeRef<SubTexture>(tex);
        subTex->SetRegion({indecies.x * tileSize.x, indecies.y * tileSize.y, tileSize.x * cellSize.x, tileSize.y * cellSize.y});
        return subTex;
    }

    SubTexture::SubTexture(Ref<Texture> tex)
        : m_mainTexture(tex)
    {
        m_coordinates[0] = {0.0f, 1.0f};
        m_coordinates[1] = {0.0f, 0.0f};
        m_coordinates[2] = {1.0f, 0.0f};
        m_coordinates[3] = {1.0f, 1.0f};
    }

    void SubTexture::SetRegion(const TextureRect &r)
    {
        auto size = m_mainTexture->GetSize();

        m_coordinates[0] = {r.left / size.x, (r.bottom + r.height) / size.y};
        m_coordinates[1] = {r.left / size.x, r.bottom / size.y};
        m_coordinates[2] = {(r.left + r.width) / size.x, r.bottom / size.y};
        m_coordinates[3] = {(r.left + r.width) / size.x, (r.bottom + r.height) / size.y};
    }

} // namespace df