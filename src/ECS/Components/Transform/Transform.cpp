#include "Transform.hpp"

namespace Voxel {
    Transform::Transform()
    {
        this->_position = glm::vec3(0.0f);
        this->_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        this->_scale = glm::vec3(1.0f);
        this->_modelMatrix = glm::mat4(1.0f);
        this->_isDirty = true;
    }

    glm::mat4 Transform::getModelMatrix() {
        if (this->_isDirty) {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), this->_position);
            glm::mat4 rotation = glm::mat4_cast(this->_rotation);
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), this->_scale);

            this->_modelMatrix = translation * rotation * scale;
            this->_isDirty = false;
        }
        return this->_modelMatrix;
    }

    void Transform::translate(glm::vec3 offset) {
        this->_position += offset;
        this->_isDirty = true;
    }

    void Transform::rotate(glm::vec3 axis, float angle) {
        this->_rotation = glm::rotate(this->_rotation, angle, axis);
        this->_isDirty = true;
    }

    void Transform::lookAt(glm::vec3 target) {
        glm::vec3 direction = glm::normalize(target - this->_position);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(up, direction));
        up = glm::cross(direction, right);

        this->_rotation = glm::quat_cast(glm::mat4(
            right.x, up.x, direction.x, 0.0f,
            right.y, up.y, direction.y, 0.0f,
            right.z, up.z, direction.z, 0.0f,
            0.0f,   0.0f,  0.0f,      1.0f
        ));
        this->_isDirty = true;
    }

    glm::vec3 Transform::getPosition() const {
        return this->_position;
    }

    glm::quat Transform::getRotation() const {
        return this->_rotation;
    }

    glm::vec3 Transform::getScale() const {
        return this->_scale;
    }

    void Transform::setPosition(const glm::vec3& position) {
        this->_position = position;
        this->_isDirty = true;
    }

    void Transform::setRotation(const glm::quat& rotation) {
        this->_rotation = rotation;
        this->_isDirty = true;
    }

    void Transform::setScale(const glm::vec3& scale) {
        this->_scale = scale;
        this->_isDirty = true;
    }
}
