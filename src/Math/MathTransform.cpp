#include "MathTransform.hpp"

namespace Voxel {
    glm::mat4 MathTransform::createModelMatrix(glm::vec3 position, glm::quat rotation, glm::vec3 scale) {
        glm::mat4 mat(1.0f);

        mat = glm::translate(mat, position);
        mat *= glm::mat4_cast(rotation);
        mat = glm::scale(mat, scale);

        return mat;
    }

    glm::mat4 MathTransform::createViewMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
        return glm::lookAt(position, target, up);
    }

    glm::mat4 MathTransform::createPerspectiveMatrix(float fov, float aspect, float near, float far) {
        return glm::perspective(fov, aspect, near, far);
    }

    glm::mat4 MathTransform::createOrthographicMatrix(float left, float right, float bottom, float top, float near, float far) {
        return glm::ortho(left, right, bottom, top, near, far);
    }
}
