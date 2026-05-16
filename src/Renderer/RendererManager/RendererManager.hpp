#pragma once

#include "../Mesh/Mesh.hpp"
#include "../Material/Material.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace Krio {
    struct DrawCall {
        const Mesh* mesh;
        const Material* material;
        glm::mat4 transform;
    };

    class RendererManager {
        public:
            RendererManager() = default;
            ~RendererManager();

            void submitMesh(const Mesh* mesh, const Material* material, const glm::mat4& transform);
            void submitCamera();
            void render();
            uint32_t getDrawCallCount() const;
            uint32_t getTriangleCount() const;

        private:
            std::vector<DrawCall> _opaqueDrawCalls;
            std::vector<DrawCall> _transparentDrawCalls;
            uint32_t _drawCallCount = 0;
            uint32_t _triangleCount = 0;
    };
}
