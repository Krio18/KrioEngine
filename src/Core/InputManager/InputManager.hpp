#pragma once

#include "../Logger/Logger.hpp"

namespace Krio {
    class InputManager {
        public:
            InputManager();
            ~InputManager() = default;

            void pollInput();
            void reset();

        private:
    };
}