#pragma once

#include "../Mesh/Mesh.hpp"
#include "../Material/Material.hpp"
#include "../../Renderer/CameraManager/CameraManager.hpp"

#include <algorithm>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <cstdint>

namespace Voxel {
    struct DrawCall {
        std::shared_ptr<const Mesh> mesh;
        std::shared_ptr<const Material> material;
        glm::mat4 transform;
    };

    struct Camera;

    class RendererManager {
        public:
            RendererManager() = default;
            ~RendererManager();

            void submitMesh(std::shared_ptr<const Mesh> mesh, std::shared_ptr<const Material> material, const glm::mat4& transform);
            void submitCamera(std::shared_ptr<const Camera> camera);
            void render();
            uint32_t getDrawCallCount() const;
            uint32_t getTriangleCount() const;

        private:
            std::vector<DrawCall> _opaqueDrawCalls;
            std::vector<DrawCall> _transparentDrawCalls;
            uint32_t _drawCallCount = 0;
            uint32_t _triangleCount = 0;
            std::shared_ptr<const Camera> _activeCamera = nullptr;
    };
}
