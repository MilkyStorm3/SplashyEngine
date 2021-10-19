/* #include "Pch.h"
#include "AssetManager.hpp"
#include <random>
#include <sstream>
namespace ant
{
    UUID AssetManager::GenUUID()
    {

        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, 15);
        static std::uniform_int_distribution<> dis2(8, 11);

        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++)
        {
            ss << dis(gen);
        }

        for (i = 0; i < 4; i++)
        {
            ss << dis(gen);
        }

        for (i = 0; i < 3; i++)
        {
            ss << dis(gen);
        }

        ss << dis2(gen);
        for (i = 0; i < 3; i++)
        {
            ss << dis(gen);
        }

        for (i = 0; i < 12; i++)
        {
            ss << dis(gen);
        };

        return UUID(ss.str());
    }

    bool AssetManager::RegisterTexture(Ref<Texture> tex, UUID id)
    {

        if (id.id.empty())
            id = GenUUID();

        if (m_textures[id])
        {
            CORE_ASSERT(false, "Resource id already occupied: " + id.id);
            return false;
        }

        m_textures[id] = tex;
        return true;
    }

    bool AssetManager::RegisterTexture(const std::string &filepath, UUID id = UUID(""))
    {

        if (id.id.empty())
            id = GenUUID();

        if (m_textures[id])
        {
            CORE_ASSERT(false, "Resource id already occupied: " + id.id);
            return false;
        }

        m_textures[id] = Texture::Create(filepath);
        return true;
    }
} */