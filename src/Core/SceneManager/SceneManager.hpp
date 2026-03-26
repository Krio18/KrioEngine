#pragma once

#include "../Logger.hpp"

namespace Krio {
    class SceneManager {
        public:
            SceneManager();
            ~SceneManager() = default;

            void update(double deltaTime);

        private:
    };
}