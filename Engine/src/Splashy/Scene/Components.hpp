#pragma once
#include <glm/vec4.hpp>
#include <string>

namespace ant
{

    struct LabelCompoment
    {
        LabelCompoment(const std::string &label) : Label(label) {}
        std::string Label;
    };

}
