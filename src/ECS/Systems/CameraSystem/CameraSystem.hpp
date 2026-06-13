#pragma once

#include "../../../Renderer/CameraManager/CameraManager.hpp"
#include "../../../Renderer/Camera/Camera.hpp"
#include "../../Registry/Registry.hpp"
#include "../../Components/Transform/Transform.hpp"
#include "../../Components/Tag/Tag.hpp"

namespace Voxel {
    class CameraSystem {
        public:
            CameraSystem(CameraManager& cameraManager);
            ~CameraSystem() = default;

            void update(Registry& registry);

        private:
            CameraManager& _cameraManager;
    };
}
