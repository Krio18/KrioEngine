#include "InputManager.hpp"
#include <SDL2/SDL.h>

namespace Voxel {
    InputManager::InputManager()
        : _currentKeys(), _previousKeys(), _mouseDeltaX(0), _mouseDeltaY(0), _mouseScroll(0.0f), _mouseButtons(0)
    {
        Logger::info("InputManager created");
    }


    void InputManager::pollInput() {
        this->_mouseScroll = 0.0f;
        memcpy(this->_previousKeys.data(), this->_currentKeys.data(), SDL_NUM_SCANCODES);

        const uint8_t* keyboardState = SDL_GetKeyboardState(nullptr);
        memcpy(this->_currentKeys.data(), keyboardState, SDL_NUM_SCANCODES * sizeof(uint8_t));

        this->_mouseButtons = SDL_GetRelativeMouseState(&_mouseDeltaX, &_mouseDeltaY);
    }

    void InputManager::reset() {}

    bool InputManager::isKeyPressed(Key key) const {
        long unsigned int scanCode = toScancode(key);
        if (scanCode >= this->_currentKeys.size())
            return false;

        return this->_currentKeys[scanCode] != 0;
    }

    bool InputManager::isKeyDown(Key key) const {
        long unsigned int scanCode = toScancode(key);
        if (scanCode >= this->_currentKeys.size())
            return false;

        return this->_currentKeys[scanCode] != 0 && this->_previousKeys[scanCode] == 0;
    }

    bool InputManager::isKeyUp(Key key) const {
        long unsigned int scanCode = toScancode(key);
        if (scanCode >= this->_currentKeys.size())
            return false;

        return this->_previousKeys[scanCode] != 0 && this->_currentKeys[scanCode] == 0;
    }

    glm::vec2 InputManager::getMouseDelta() const {
        return glm::vec2{this->_mouseDeltaX, this->_mouseDeltaY};
    }

    float InputManager::getMouseScroll() const {
        return this->_mouseScroll;
    }

    bool InputManager::isMouseButtonPressed(MouseButton button) const {
        switch (button) {
            case MouseButton::Left: return this->_mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT);
            case MouseButton::Right: return this->_mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT);
            case MouseButton::Middle: return this->_mouseButtons & SDL_BUTTON(SDL_BUTTON_MIDDLE);
            default: return false;
        }
    }

    void InputManager::addScrollDelta(float delta) {
        this->_mouseScroll += delta;
    }


    int InputManager::toScancode(Key key) const {
        switch (key) {
            case Key::W: return SDL_SCANCODE_W;
            case Key::A: return SDL_SCANCODE_A;
            case Key::S: return SDL_SCANCODE_S;
            case Key::D: return SDL_SCANCODE_D;
            case Key::Space: return SDL_SCANCODE_SPACE;
            case Key::LeftShift: return SDL_SCANCODE_LSHIFT;
            case Key::Escape: return SDL_SCANCODE_ESCAPE;
            default: return SDL_SCANCODE_UNKNOWN;
        }
    }
}