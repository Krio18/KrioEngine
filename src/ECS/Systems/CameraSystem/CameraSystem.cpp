/*
** EPITECH PROJECT, 2026
** VoxelEngine
** File description:
** CameraSystem
*/

#include "CameraSystem.hpp"

namespace Voxel {
    CameraSystem::CameraSystem(CameraManager& cameraManager) : _cameraManager(cameraManager) {}

    void CameraSystem::update(Registry& registry) {
        this->_cameraManager.clearCameras();

        auto cameraList = registry.view<Transform, Camera>();
        for (auto [entity, transform, camera] : cameraList.each()) {
            Entity wrappedEntity(entity, &registry);
            Tag* tag = nullptr;
            if (wrappedEntity.hasComponent<Tag>()) {
                tag = wrappedEntity.getComponent<Tag>();
            }
            if (tag && !tag->enableEntity) {
                continue;
            }

            camera.position = transform.getPosition();
            camera.rotation = transform.getRotation();

            auto cameraPtr = std::make_shared<Camera>(camera);
            this->_cameraManager.registerCamera(cameraPtr);

            if (tag && tag->tag == "MainCamera") {
                this->_cameraManager.setMainCamera(cameraPtr);
            }
        }
    }
}
