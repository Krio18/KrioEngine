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
            bool isMouseButtonDown(MouseButton button) const;
            bool isMouseButtonUp(MouseButton button) const;
            glm::vec2 getMousePosition() const;

            void addScrollDelta(float delta);
            void resetScroll();

        private:
            std::array<uint8_t, KEY_COUNT> _currentKeys;
            std::array<uint8_t, KEY_COUNT> _previousKeys;
            int _mouseDeltaX;
            int _mouseDeltaY;
            float _mouseScroll;
            uint32_t _mouseButtons;
            uint32_t _previousMouseButtons;
            int _mouseX;
            int _mouseY;

            int toScancode(Key key) const;
    };
}