#pragma once
#include "Core/Core.hpp"
#include <vector>
#include <string>
#include <glm/vec2.hpp>

namespace df
{

    class Texture
    {
    public:
        inline static Ref<Texture> Create() { return MakeRef<Texture>(); }
        static Ref<Texture> Create(const std::string &filePath);
        static Ref<Texture> Create(const glm::ivec2 &dimensions, uint32_t channelCount = 4);

        inline static Ref<Texture> Create(int width, int height, uint32_t channelCount = 4) { return Create({width, height}, channelCount); }

        Texture(bool keepLocalBuffer = false);
        ~Texture();

        void LoadFromFile(const std::string &filePath);
        void Upload(uint32_t slot);
        void ClearLocalBuffer();
        void SetKeepLocalBuffer(bool keep = true) { m_keepLocalBuffer = keep; }
        void SetData(void *data, int32_t size);
        inline const glm::ivec2 &GetSize() const { return m_dimensions; }
        int32_t GetSlot() { return m_slot; }

    private:
        void SetFormat(uint32_t channelCount);

    private:
        static std::unordered_map<std::string,Ref<Texture>> s_loadedTextures;

        uint32_t m_glId;
        uchar *m_rawData = nullptr;
        glm::ivec2 m_dimensions;
        bool m_keepLocalBuffer;
        bool m_uploaded = false;
        int32_t m_slot = -1;
        uint32_t m_internalFormat;
        uint32_t m_subimageFormat;
    };

    struct TextureRect
    {
        float left, bottom;
        float width, height;
    };

    class SubTexture
    {
    public:
        friend class OldQuad;
        friend class Renderer2D;

        static Ref<SubTexture> Create(Ref<Texture> tex, const glm::vec2& indecies, const glm::vec2& tileSize, const glm::vec2& cellSize = {1,1});

        SubTexture(Ref<Texture> tex);
        ~SubTexture(){}

        void SetRegion(const TextureRect &region);        
        Ref<Texture> GetTexture() const { return m_mainTexture; }

    private:
        std::array<glm::vec2, 4> GetCoordinateData() const { return m_coordinates; }

    private:
        Ref<Texture> m_mainTexture;
        std::array<glm::vec2, 4> m_coordinates;
    };

} // namespace df
