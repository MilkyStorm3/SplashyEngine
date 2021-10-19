#include "Pch.h"
#include "Scene/Scene.hpp"

namespace ant
{
    Entity Scene::RegisterEntity()
    {
        return {this, m_registry.create()};
    }
}