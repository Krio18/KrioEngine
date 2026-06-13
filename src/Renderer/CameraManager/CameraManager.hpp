#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "../Camera/Camera.hpp"
#include "../../Core/Logger/Logger.hpp"

namespace Voxel {
    class CameraManager {
        public:
            CameraManager();
            ~CameraManager();

            void registerCamera(std::shared_ptr<Camera> cam);
            void unregisterCamera(std::shared_ptr<Camera> cam);
            void setMainCamera(std::shared_ptr<Camera> cam);
            std::shared_ptr<Camera> getMainCamera() const;
            std::vector<std::shared_ptr<Camera>> getAllCameras() const;
            bool validateCamera(const Camera& cam) const;
            void clearCameras();

        private:
            std::vector<std::shared_ptr<Camera>> _cameras;
            std::shared_ptr<Camera> _mainCamera = nullptr;
    };
}

