#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Voxel {
    class MathTransform {
        public:
            MathTransform() = default;
            ~MathTransform() = default;

            static glm::mat4 createModelMatrix(glm::vec3 position, glm::quat rotation, glm::vec3 scale);
            static glm::mat4 createViewMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 up);
            static glm::mat4 createPerspectiveMatrix(float fov, float aspect, float near, float far);
            static glm::mat4 createOrthographicMatrix(float left, float right, float bottom, float top, float near, float far);

        private:
    };
}
