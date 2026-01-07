#pragma once

#include "../Logger.hpp"

class InputManager {
    public:
        InputManager();
        ~InputManager() = default;

        void pollInput();
        void reset();

    private:
};