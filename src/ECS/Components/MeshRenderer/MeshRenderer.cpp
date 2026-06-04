#include "MeshRenderer.hpp"

namespace Voxel {
    MeshRenderer::MeshRenderer() :
    _meshName(""), _materialName(""), _castShadows(true),
    _receiveShadows(true), _renderingLayerMask(0xFFFFFFFF),
    _boundsMin(glm::vec3(0.0f)), _boundsMax(glm::vec3(0.0f)) {}
}
