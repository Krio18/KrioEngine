#include "Camera.hpp"

namespace Voxel {
    glm::mat4 Camera::getViewMatrix() const {
        glm::mat4 model = MathTransform::createModelMatrix(this->position, this->rotation, glm::vec3(1.0f));
        return glm::inverse(model);
    }

    glm::mat4 Camera::getProjectionMatrix() const {
        if (this->projectionType == ProjectionType::Perspective) {
            return MathTransform::createPerspectiveMatrix(glm::radians(this->fovDegrees), this->aspect, this->zNear, this->zFar);
        }
        return MathTransform::createOrthographicMatrix(this->left, this->right, this->bottom, this->top, this->zNear, this->zFar);
    }

    glm::mat4 Camera::getViewProjectionMatrix() const {
        glm::mat4 V = getViewMatrix();
        glm::mat4 P = getProjectionMatrix();
        return P * V;
    }

    glm::vec3 Camera::getPosition() const {
        return position;
    }

    int Camera::getPriority() const {
        return priority;
    }
}
