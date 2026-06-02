#pragma once

#include "../../Camera/Camera.hpp"

#include <glm/glm.hpp>
#include <algorithm>
#include <memory>

namespace Voxel {
    class FollowCameraController {
        public:
            FollowCameraController(Camera& camera);
            ~FollowCameraController() = default;

            void update(float deltaTime);

            void setTarget(std::shared_ptr<glm::vec3> target);
            void setOffset(const glm::vec3& offset);
            void setDamping(float damping);

        private:
            Camera& _camera;
            std::shared_ptr<glm::vec3> _target;
            glm::vec3 _offset;
            float _damping;
    };
}