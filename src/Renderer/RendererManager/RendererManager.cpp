#include "RendererManager.hpp"

namespace Krio {
    RendererManager::~RendererManager() {
        this->_opaqueDrawCalls.clear();
        this->_transparentDrawCalls.clear();
    }

    void RendererManager::submitMesh(const Mesh* mesh, const Material* material, const glm::mat4& transform) {
        if (material->isTransparent()) {
            this->_transparentDrawCalls.push_back({mesh, material, transform});
        } else {
            this->_opaqueDrawCalls.push_back({mesh, material, transform});
        }
    }

    void RendererManager::submitCamera() {}

    uint32_t RendererManager::getDrawCallCount() const {
        return this->_drawCallCount;
    }

    uint32_t RendererManager::getTriangleCount() const {
        return this->_triangleCount;
    }

    void RendererManager::render() {
        this->_drawCallCount = 0;
        this->_triangleCount = 0;

        for (const auto& drawCall : this->_opaqueDrawCalls) {
            drawCall.material->submit();
            bgfx::setTransform(&drawCall.transform);
            drawCall.mesh->draw(0, drawCall.material->getShaderHandle());
            this->_drawCallCount += 1;
            this->_triangleCount += drawCall.mesh->getIndexCount() / 3;
        }
        for (const auto& drawCall : this->_transparentDrawCalls) {
            drawCall.material->submit();
            bgfx::setTransform(&drawCall.transform);
            drawCall.mesh->draw(0, drawCall.material->getShaderHandle());
            this->_drawCallCount += 1;
            this->_triangleCount += drawCall.mesh->getIndexCount() / 3;
        }
        this->_opaqueDrawCalls.clear();
        this->_transparentDrawCalls.clear();
    }
}
