#pragma once
#include "Render/Primitive.hpp"
#include <glm/vec4.hpp>
#include "Graphics/Texture.hpp"

#include "Render/Transform.hpp" //! Trabsform component

namespace df
{

    struct LabelCompoment
    {
        LabelCompoment(const std::string &label) : Label(label) {}
        std::string Label;
    };

    struct SpriteRenderComponent : public Quad
    {
        SpriteRenderComponent(const glm::vec4 &color = {1.f, 1.f, 1.f, 1.f}) : Quad(color) {}
    };

    struct TextureComponent
    {
        Ref<SubTexture> Texture;
    };

}
