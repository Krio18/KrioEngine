#pragma once

#include <bgfx/bgfx.h>

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

            static void init();

        private:
            static bgfx::VertexLayout _vertexLayout;
            bgfx::VertexBufferHandle _vbh;
            bgfx::IndexBufferHandle _ibh;

    };
}
