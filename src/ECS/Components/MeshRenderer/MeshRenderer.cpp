#include "MeshRenderer.hpp"

namespace Voxel {
    MeshRenderer::MeshRenderer() :
    meshName(""), materialName(""), castShadows(true),
    receiveShadows(true), renderingLayerMask(0xFFFFFFFF),
    boundsMin(glm::vec3(0.0f)), boundsMax(glm::vec3(0.0f)) {}
}
