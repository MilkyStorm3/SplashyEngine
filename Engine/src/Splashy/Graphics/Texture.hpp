#pragma once
#include "Core/Core.hpp"
#include <filesystem>
#include <initializer_list>
#include <glm/vec4.hpp>

namespace ant
{
    enum class TextureParam
    {
        None = 0,
        MagnificationFilter,
        MinificationFilter,

        XWrap,
        YWrap,
        ZWrap
    };

    enum class TextureParamValue
    {
        None = 0,

        Nearest,
        Linear,
        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear,

        ClampToEdge,
        ClampToBorder,
        MirroredRepeat,
        Repeat,
        MirrorClampToEdge
    };

    class Texture
    {
    public:
        Texture() {}
        virtual ~Texture() {}

        Texture(const Texture &other) = delete;

        virtual bool IsLoaded() const = 0;
        virtual uint32_t GetRendererId() const = 0;
        virtual void Bind(uint32_t slot = 0) const = 0;
        virtual void SetData(void *data, uint32_t size) = 0;

        virtual void SetBorderColor(const glm::vec4 &color) = 0;
        virtual void SetLevelOfDetail(float min = -1000.f, float max = 1000.f) = 0;
        virtual void SetParameter(TextureParam param, TextureParamValue value) = 0;
    };

    class Texture2D : public Texture
    {
    public:
        /**
         * @param path: Path to a texture image
         * @retval Ref to uploaded ready to bind texture
         */
        static Ref<Texture2D> Create(const std::filesystem::path &path);
        /**
         * @param path: Dimensions of new texture
         * @retval Ref to texture prepared to recieve data by @ref Texture::SetData(void *data, uint32_t size)
         */
        static Ref<Texture2D> Create(uint32_t width, uint32_t height);

        virtual uint32_t GetSizeX() const = 0;
        virtual uint32_t GetSizeY() const = 0;
    };

} // namespace ant
