#include "RenderManager.hpp"

namespace Krio {
    RenderManager::RenderManager() {
        this->_angle = 0.0f;

        Logger::info("RenderManager created");
    }

    RenderManager::~RenderManager() {
        Logger::info("RenderManager destroyed");
    }

    void RenderManager::init() {
        this->_shader = std::make_unique<Shader>(
            "build/shaders/spirv/v_simple.sc.bin",
            "build/shaders/spirv/f_simple.sc.bin"
        );

        this->_cubeMesh = Mesh::createCube();
    }

    void RenderManager::render( double deltaTime) {
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::touch(0);

        this->_angle += 1.0f * deltaTime;
        glm::mat4 model = Transform::createModelMatrix(glm::vec3(0.0f), glm::angleAxis(this->_angle, glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f));
        glm::mat4 view = Transform::createViewMatrix(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 proj = Transform::createPerspectiveMatrix(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        bgfx::setViewTransform(0, &view, &proj);
        bgfx::setTransform(&model);

        this->_cubeMesh.draw(0, this->_shader->getProgramHandle());
    }
}
