#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "../ECS/Entity/Entity.hpp"
#include "../ECS/Registry/Registry.hpp"
#include "../ECS/Components/Tag/Tag.hpp"
#include "../ECS/Systems/RenderSystem/RenderSystem.hpp"
#include "../ECS/Systems/CameraSystem/CameraSystem.hpp"
#include "../Renderer/MeshManager/MeshManager.hpp"
#include "../Renderer/MaterialManager/MaterialManager.hpp"
#include "../Core/RenderManager/RenderManager.hpp"
#include "../Renderer/CameraManager/CameraManager.hpp"

namespace Voxel {
    class SceneSerializer;

    class Scene {
        public:
            Scene(const std::string& name,
                  MeshManager& meshManager,
                  MaterialManager& materialManager,
                  RenderManager& renderManager,
                  CameraManager& cameraManager);
            ~Scene() = default;

            const std::string& getName() const;

            Entity createEntity(const std::string& name);
            void destroyEntity(Entity entity);

            Entity getParent(Entity entity) const;
            std::vector<Entity> getChildren(Entity entity) const;
            void setParent(Entity child, Entity parent);

            void update(double deltaTime);

            glm::vec3 ambientLightColor;
            bool fogEnabled;
            std::string skyboxName;

        private:
            friend class SceneSerializer;

            std::string _name;
            Registry _registry;
            RenderSystem _renderSystem;
            CameraSystem _cameraSystem;
            std::unordered_map<entt::entity, entt::entity> _parentMap;
            std::unordered_map<entt::entity, std::vector<entt::entity>> _childrenMap;
    };
}
