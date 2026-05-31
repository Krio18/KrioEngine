#pragma once

#include "../../Camera/Camera.hpp"
#include "../../../Platform/InputManager/InputManager.hpp"
#include "../../../Core/Logger/Logger.hpp"

#include <algorithm>

namespace Voxel {
    class FPSCameraController {
        public:
            FPSCameraController(Camera& camera);
            ~FPSCameraController() = default;

            void update(float deltaTime, const InputManager& inputmanager);

        private:
            Camera& _camera;
            float _yaw;
            float _pitch;
            float _moveSpeed;
            float _sensitivity;
    };
}
