#include "FollowCameraController.hpp"

namespace Voxel {
    FollowCameraController::FollowCameraController(Camera& camera)
        : _camera(camera), _target(nullptr), _offset(0.0f), _damping(5.0f) {}

    void FollowCameraController::update(float deltaTime) {
        if (!this->_target)
            return;

        glm::vec3 targetPosition = (*this->_target) + this->_offset;

        glm::vec3 current = this->_camera.position;
        float t = std::clamp(this->_damping * deltaTime, 0.0f, 1.0f);
        this->_camera.position = glm::mix(current, targetPosition, t);

        this->_camera.orientation = glm::quatLookAt(glm::normalize((*this->_target) - this->_camera.position), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void FollowCameraController::setTarget(glm::vec3* target) {
        this->_target = target;
    }

    void FollowCameraController::setOffset(const glm::vec3& offset) {
        this->_offset = offset;
    }

    void FollowCameraController::setDamping(float damping) {
        this->_damping = damping;
    }
}
