#include "Scene.hpp"
#include "../Core/Logger/Logger.hpp"

#include <algorithm>

namespace Voxel {
    Scene::Scene(const std::string& name,
                 MeshManager& meshManager,
                 MaterialManager& materialManager,
                 RenderManager& renderManager,
                 CameraManager& cameraManager) :
        _name(name),
        _renderSystem(meshManager, materialManager, renderManager),
        _cameraSystem(cameraManager)
    {
        ambientLightColor = glm::vec3(0.1f);
        fogEnabled = false;
        Logger::info("Scene created: " + _name);
    }

    const std::string& Scene::getName() const {
        return this->_name;
    }

    Entity Scene::createEntity(const std::string& name) {
        Entity entity = this->_registry.createEntity();
        Tag tag;
        tag.entityName = name;
        tag.tag = name;
        this->_registry.addComponent<Tag>(entity, tag);
        return entity;
    }

    void Scene::destroyEntity(Entity entity) {
        if (!entity.isValid()) {
            return;
        }

        entt::entity entityId = entity.getID();

        auto parentIt = this->_parentMap.find(entityId);
        if (parentIt != this->_parentMap.end()) {
            auto& siblings = this->_childrenMap[parentIt->second];
            siblings.erase(std::remove(siblings.begin(), siblings.end(), entityId), siblings.end());
            this->_parentMap.erase(parentIt);
        }

        auto childrenIt = this->_childrenMap.find(entityId);
        if (childrenIt != this->_childrenMap.end()) {
            for (auto childId : childrenIt->second) {
                this->_parentMap.erase(childId);
            }
            this->_childrenMap.erase(childrenIt);
        }

        for (auto it = this->_childrenMap.begin(); it != this->_childrenMap.end();) {
            auto& children = it->second;
            children.erase(std::remove(children.begin(), children.end(), entityId), children.end());
            if (children.empty()) {
                it = this->_childrenMap.erase(it);
            } else {
                ++it;
            }
        }

        this->_registry.destroyEntity(entity);
    }

    Entity Scene::getParent(Entity entity) const {
        if (!entity.isValid()) {
            return Entity(entt::null, nullptr);
        }

        auto it = this->_parentMap.find(entity.getID());
        if (it == this->_parentMap.end()) {
            return Entity(entt::null, nullptr);
        }

        return Entity(it->second, const_cast<Registry*>(&this->_registry));
    }

    std::vector<Entity> Scene::getChildren(Entity entity) const {
        std::vector<Entity> children;
        if (!entity.isValid()) {
            return children;
        }

        auto it = this->_childrenMap.find(entity.getID());
        if (it == this->_childrenMap.end()) {
            return children;
        }

        children.reserve(it->second.size());
        for (auto childId : it->second) {
            children.emplace_back(childId, const_cast<Registry*>(&this->_registry));
        }

        return children;
    }

    void Scene::setParent(Entity child, Entity parent) {
        if (!child.isValid() || !parent.isValid() || child == parent) {
            return;
        }

        entt::entity childId = child.getID();
        entt::entity parentId = parent.getID();

        auto oldParentIt = this->_parentMap.find(childId);
        if (oldParentIt != this->_parentMap.end()) {
            auto& oldSiblings = this->_childrenMap[oldParentIt->second];
            oldSiblings.erase(std::remove(oldSiblings.begin(), oldSiblings.end(), childId), oldSiblings.end());
        }

        this->_parentMap[childId] = parentId;
        auto& children = this->_childrenMap[parentId];
        if (std::find(children.begin(), children.end(), childId) == children.end()) {
            children.push_back(childId);
        }
    }

    void Scene::update(double /*deltaTime*/) {
        this->_cameraSystem.update(this->_registry);
        this->_renderSystem.update(this->_registry);
    }
}
