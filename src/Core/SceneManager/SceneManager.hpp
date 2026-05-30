#pragma once

#include "../Logger/Logger.hpp"

namespace Voxel {
    class SceneManager {
        public:
            SceneManager();
            ~SceneManager() = default;

            void update(double deltaTime);

        private:
    };
}