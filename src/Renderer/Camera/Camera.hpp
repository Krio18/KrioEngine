#pragma once

#include "../../Math/Transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Voxel {
    enum class ProjectionType {
        Perspective,
        Orthographic
    };

    struct Camera {
        glm::vec3 position = glm::vec3(0.0f);
        glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

        ProjectionType projectionType = ProjectionType::Perspective;
        float fovDegrees = 60.0f;
        float aspect = 4.0f / 3.0f;
        float zNear = 0.1f;
        float zFar = 100.0f;

        float left = -1.0f;
        float right = 1.0f;
        float bottom = -1.0f;
        float top = 1.0f;

        int priority = 0;

        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;
        glm::mat4 getViewProjectionMatrix() const;
        glm::vec3 getPosition() const;
        int getPriority() const;
    };
}
