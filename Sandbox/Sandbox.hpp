#pragma once

#include "../src/Core/Engine/Engine.hpp"
#include "../src/Renderer/Controllers/FollowCameraController/FollowCameraController.hpp"
#include "../src/Math/Transform.hpp"

namespace Voxel {
    class Sandbox : public Engine {
        public:
            Sandbox();
            ~Sandbox() = default;

            Sandbox(const Sandbox&) = delete;
            Sandbox& operator=(const Sandbox&) = delete;

        protected:
            void onInit() override;
            void onUpdate() override;
            void onShutdown() override;

        private:
            std::unique_ptr<FollowCameraController> _cameraController;
            glm::vec3 _targetPosition;
            float _targetAngle;
            float _mainAngle;
            Mesh* _cubeMesh = nullptr;
            std::shared_ptr<Material> _material = nullptr;
    };
}
