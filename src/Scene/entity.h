#ifndef ENTITY_H
#define ENTITY_H

#include <entt/entt.hpp>
#include "scene.h"

class Entity
{
public:
    Entity() {};
    ~Entity() {};

    // Constructor
    Entity(entt::entity entity, Scene *scene) : m_EntityID(entity), m_Scene(scene) {};

    // Get entity ID
    entt::entity ID() const { return m_EntityID; }

    // Check if component exists
    template <typename T>
    bool HasComponent()
    {
        return m_Scene->Reg().any_of<T>(m_EntityID);
    }

    // Add component
    template <typename T, typename... Args>
    T &AddComponent(Args &&...args)
    {
        return m_Scene->Reg().emplace<T>(m_EntityID, std::forward<Args>(args)...);
    }

    // Get component
    template <typename T>
    T &GetComponent()
    {
        return m_Scene->Reg().get<T>(m_EntityID);
    }

    template <typename T>
    void RemoveComponent()
    {
        m_Scene->Reg().remove<T>(m_EntityID);
    }

    operator bool() const { return m_EntityID != entt::null; }

private:
    entt::entity m_EntityID{entt::null};
    Scene *m_Scene = nullptr;
};

#endif