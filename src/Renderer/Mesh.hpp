#pragma once

#include <bgfx/bgfx.h>
#include <vector>
#include <algorithm>


struct Vertex {
    float x, y, z;
    float nx, ny, nz;
    float u, v;
    uint32_t color;
};

namespace Krio {
    class Mesh {
        public:
            Mesh() = default;
            ~Mesh();

            void draw(bgfx::ViewId viewId, bgfx::ProgramHandle program);

            static void init();
            static Mesh createTriangle();
            static Mesh createQuad();
            static Mesh createCube();

        private:
            static bgfx::VertexLayout _vertexLayout;
            bgfx::VertexBufferHandle _vbh;
            bgfx::IndexBufferHandle _ibh;

            float _minX, _minY, _minZ;
            float _maxX, _maxY, _maxZ;
    };
}
