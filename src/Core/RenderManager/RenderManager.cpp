#include "RenderManager.hpp"

namespace Krio {
    RenderManager::RenderManager() {
        Logger::info("RenderManager created");
    }

    void RenderManager::init() {
        this->_shader = std::make_unique<Shader>(
            "build/shaders/spirv/v_simple.sc.bin",
            "build/shaders/spirv/f_simple.sc.bin"
        );

        this->_triangleMesh = Mesh::createTriangle();
    }

    void RenderManager::render() {
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::touch(0);
        this->_triangleMesh.draw(0, this->_shader->getProgramHandle());
    }
}