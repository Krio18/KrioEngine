#include "FPSCameraController.hpp"

namespace Voxel {
    FPSCameraController::FPSCameraController(Camera& camera)
        : _camera(camera), _yaw(0.0f), _pitch(0.0f), _moveSpeed(5.0f), _sensitivity(0.1f) {}

    void FPSCameraController::update(float deltaTime, const InputManager& inputmanager) {
        glm::vec2 mouseDelta = inputmanager.getMouseDelta();
        this->_yaw -= mouseDelta.x * this->_sensitivity;
        this->_pitch -= mouseDelta.y * this->_sensitivity;

        this->_pitch = std::clamp(this->_pitch, -89.0f, 89.0f);

        glm::quat yawRotation = glm::angleAxis(glm::radians(this->_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat pitchRotation = glm::angleAxis(glm::radians(this->_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        this->_camera.orientation = yawRotation * pitchRotation;

        glm::vec3 forward = this->_camera.orientation * glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 right = this->_camera.orientation * glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 up = this->_camera.orientation * glm::vec3(0.0f, 1.0f, 0.0f);


        if (inputmanager.isKeyPressed(Key::W)) this->_camera.position += forward * this->_moveSpeed * deltaTime;
        if (inputmanager.isKeyPressed(Key::S)) this->_camera.position -= forward * this->_moveSpeed * deltaTime;
        if (inputmanager.isKeyPressed(Key::A)) this->_camera.position -= right * this->_moveSpeed * deltaTime;
        if (inputmanager.isKeyPressed(Key::D)) this->_camera.position += right * this->_moveSpeed * deltaTime;
        if (inputmanager.isKeyPressed(Key::Space)) this->_camera.position += up * this->_moveSpeed * deltaTime;
        if (inputmanager.isKeyPressed(Key::LeftShift)) this->_camera.position -= up * this->_moveSpeed * deltaTime;
    }
}