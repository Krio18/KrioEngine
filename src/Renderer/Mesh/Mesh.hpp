#pragma once

#include <bgfx/bgfx.h>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <mutex>

namespace Voxel {
    struct Vertex {
        float x, y, z;
        float nx, ny, nz;
        float u, v;
        uint32_t color;
    };

    class Mesh {
        public:
            Mesh();
            Mesh(Mesh&& other) noexcept;
            Mesh(const Mesh&) = delete;
            Mesh& operator=(Mesh&& other) noexcept;

            ~Mesh();

            void draw(bgfx::ViewId viewId, bgfx::ProgramHandle program) const;
            uint32_t getIndexCount() const;

            static void init();
            static Mesh createTriangle();
            static Mesh createQuad();
            static Mesh createCube();

        private:
            static bgfx::VertexLayout _vertexLayout;
            static std::once_flag _layoutInitFlag;
            bgfx::VertexBufferHandle _vbh;
            bgfx::IndexBufferHandle _ibh;
            uint32_t _indexCount;

            float _minX, _minY, _minZ;
            float _maxX, _maxY, _maxZ;
    };
}
