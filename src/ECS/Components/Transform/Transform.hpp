#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Voxel {
    class Transform {
        public:
            Transform();
            ~Transform() = default;

            glm::mat4 getModelMatrix();
            void translate(glm::vec3 offset);
            void rotate(glm::vec3 axis, float angle);
            void lookAt(glm::vec3 target);

        private:
            glm::vec3 _position;
            glm::quat _rotation;
            glm::vec3 _scale;
            glm::mat4 _modelMatrix;
            bool _isDirty;
    };
}
