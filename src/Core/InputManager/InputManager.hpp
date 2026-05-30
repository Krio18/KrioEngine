#pragma once

#include "../Logger/Logger.hpp"

namespace Voxel {
    class InputManager {
        public:
            InputManager();
            ~InputManager() = default;

            void pollInput();
            void reset();

        private:
    };
}