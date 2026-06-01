#pragma once

#include "../../Camera/Camera.hpp"
#include "../../../Platform/InputManager/InputManager.hpp"

#include <algorithm>
#include <glm/glm.hpp>

namespace Voxel {
    class OrbitCameraController {
        public:
            OrbitCameraController(Camera& camera);
            ~OrbitCameraController() = default;

            void update(float deltaTime, const InputManager& inputmanager);
            void setTarget(const glm::vec3& target);

        private:
            Camera& _camera;
            glm::vec3 _target;

            float _yaw;
            float _pitch;
            float _distance;
            float _minDistance;
            float _maxDistance;
            float _minPitch;
            float _maxPitch;
            float _sensitivity;
            float _zoomSpeed;
    };
}