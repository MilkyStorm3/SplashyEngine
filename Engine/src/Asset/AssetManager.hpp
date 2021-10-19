/* #pragma once
#include <string>
#include <map>
#include "Graphics/Texture.hpp"

namespace ant
{

    struct UUID
    {
        std::string id;
        operator const std::string &() { return id; }
    };

    class AssetManager
    {
    public:
        AssetManager() {}

        ~AssetManager() {
            m_textures.clear();
        }

        static UUID GenUUID();  

        bool RegisterTexture(Ref<Texture>tex, UUID id = UUID(""));
        bool RegisterTexture(const std::string& filepath, UUID id = UUID(""));

    private:
        std::map<UUID, Ref<Texture>> m_textures;
        std::map<UUID, SubTexture> m_subTextures;


    };

    // generate uuids and use the idntify loaded resources

} */