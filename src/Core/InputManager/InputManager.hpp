#pragma once

#include "../Logger.hpp"

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