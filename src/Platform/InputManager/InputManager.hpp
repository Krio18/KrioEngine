#pragma once

#include "../../Core/Logger/Logger.hpp"

#include <glm/glm.hpp>
#include <array>
#include <cstring>

namespace Voxel {
    static constexpr size_t KEY_COUNT = 512;

    enum class Key {
        W,
        A,
        S,
        D,
        Space,
        LeftShift,
        Escape
    };

    enum class MouseButton {
        Left,
        Right,
        Middle
    };

    class InputManager {
        public:
            InputManager();
            ~InputManager() = default;

            void pollInput();
            void reset();

            bool isKeyPressed(Key key) const;
            bool isKeyDown(Key key) const;
            bool isKeyUp(Key key) const;

            glm::vec2 getMouseDelta() const;
            float getMouseScroll() const;
            bool isMouseButtonPressed(MouseButton button) const;

            void addScrollDelta(float delta);

        private:
            std::array<uint8_t, KEY_COUNT> _currentKeys;
            std::array<uint8_t, KEY_COUNT> _previousKeys;
            int _mouseDeltaX;
            int _mouseDeltaY;
            float _mouseScroll;
            uint32_t _mouseButtons;

            int toScancode(Key key) const;
    };
}