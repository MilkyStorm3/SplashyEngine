#include "Pch.h"
#include "Scene/Scene.hpp"

namespace df
{
    Entity Scene::RegisterEntity()
    {
        return {this, m_registry.create()};
    }
}