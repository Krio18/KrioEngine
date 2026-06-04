#pragma once

#include <string>
#include <cstdint>

#include <glm/glm.hpp>

namespace Voxel {
    struct MeshRenderer {
        MeshRenderer();

        std::string meshName;
        std::string materialName;

        bool castShadows;
        bool receiveShadows;
        uint32_t renderingLayerMask;
        glm::vec3 boundsMin;
        glm::vec3 boundsMax;
    };
}
