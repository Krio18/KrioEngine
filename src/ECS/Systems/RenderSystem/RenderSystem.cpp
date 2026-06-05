#include "RenderSystem.hpp"

namespace Voxel {
    RenderSystem::RenderSystem(MeshManager& meshManager, MaterialManager& materialManager, RenderManager& renderManager) :
    _meshManager(meshManager), _materialManager(materialManager), _renderManager(renderManager) {}

    void RenderSystem::update(Registry& registry) {
        auto renderedList = registry.view<Transform, MeshRenderer>();

        for (auto [entity, transform, meshRenderer] : renderedList.each()) {
            Entity wrappedEntity(entity, &registry);
            if (wrappedEntity.hasComponent<Tag>()) {
                auto* tag = wrappedEntity.getComponent<Tag>();
                if (tag && !tag->enableEntity) {
                    continue;
                }
            }

            auto mesh = this->_meshManager.get(meshRenderer.meshName);
            auto material = this->_materialManager.get(meshRenderer.materialName);

            if (mesh && material) {
                this->_renderManager.submitMesh(mesh, material, transform.getModelMatrix());
            }
        }
    }
}
