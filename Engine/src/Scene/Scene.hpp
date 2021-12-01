#pragma once
#include "Core/Core.hpp"
#include <vector>
#include <entt/entt.hpp>
namespace ant
{
    class Scene;

    class Entity
    {
    public:
        Entity(Scene *scene, entt::entity id) : m_entityId(id), m_sceneRef(scene) {}
        ~Entity() {}

        template <class... T>
        inline bool HasComponent()
        {
            return m_sceneRef->m_registry.template all_of<T...>(m_entityId);
        }

        template <class T, class... Args>
        T &AddComponent(Args &&...args)
        {
            CORE_ASSERT(!HasComponent<T>(), "Entity already has a component");
            return m_sceneRef->m_registry.template emplace<T>(m_entityId, std::forward<Args>(args)...);
        }

        template <class T>
        T &GetComponent()
        {
            CORE_ASSERT(HasComponent<T>(), "Entity has no such a component");
            return m_sceneRef->m_registry.template get<T>(m_entityId);
        }

    private:
        entt::entity m_entityId;
        Scene *m_sceneRef;
    };

    class Scene
    {
        friend class Entity;

    public:
        Scene() {}
        ~Scene() {}

        Entity RegisterEntity();
        inline std::string &Label() { return m_label; }
        void SetLabel(const std::string &lbl) { m_label = lbl; }

        inline entt::registry &GetRegistry() { return m_registry; }

    private:
        entt::registry m_registry;
        std::string m_label = "Scene";
    };

} // namespace
