#include "OrbitCameraController.hpp"

namespace Voxel {
    OrbitCameraController::OrbitCameraController(Camera& camera)
        : _camera(camera), _target(0.0f), _yaw(0.0f), _pitch(0.0f), _distance(5.0f),
        _minDistance(1.0f), _maxDistance(20.0f), _minPitch(-89.0f), _maxPitch(89.0f),
        _sensitivity(0.1f), _zoomSpeed(1.0f) {}

    void OrbitCameraController::update(float /*deltaTime*/, const InputManager& inputmanager) {
        if (inputmanager.isMouseButtonPressed(MouseButton::Right)) {
            glm::vec2 mouseDelta = inputmanager.getMouseDelta();
            this->_yaw += mouseDelta.x * this->_sensitivity;
            this->_pitch += mouseDelta.y * this->_sensitivity;

            this->_pitch = std::clamp(this->_pitch, this->_minPitch, this->_maxPitch);
        }

        float scrollDelta = inputmanager.getMouseScroll();
        if (scrollDelta != 0.0f) {
            this->_distance -= scrollDelta * this->_zoomSpeed;
            this->_distance = std::clamp(this->_distance, this->_minDistance, this->_maxDistance);
        }

        float yawRad = glm::radians(this->_yaw);
        float pitchRad = glm::radians(this->_pitch);

        glm::vec3 offset;
        offset.x = this->_distance * cos(pitchRad) * sin(yawRad);
        offset.y = this->_distance * sin(pitchRad);
        offset.z = this->_distance * cos(pitchRad) * cos(yawRad);

        this->_camera.position = this->_target + offset;
        this->_camera.rotation = glm::quatLookAt(glm::normalize(this->_target - this->_camera.position), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void OrbitCameraController::setTarget(const glm::vec3& target) {
        this->_target = target;
    }
}
