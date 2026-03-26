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
    }

    void RenderManager::render() {}
}