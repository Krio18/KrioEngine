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

            glm::vec3 getPosition() const;
            glm::quat getRotation() const;
            glm::vec3 getScale() const;

            void setPosition(const glm::vec3& position);
            void setRotation(const glm::quat& rotation);
            void setScale(const glm::vec3& scale);

        private:
            glm::vec3 _position;
            glm::quat _rotation;
            glm::vec3 _scale;
            glm::mat4 _modelMatrix;
            bool _isDirty;
    };
}
