#include "RendererManager.hpp"

namespace Voxel {
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

    void RendererManager::submitCamera(const Camera* camera) {
        this->_activeCamera = camera;
    }

    uint32_t RendererManager::getDrawCallCount() const {
        return this->_drawCallCount;
    }

    uint32_t RendererManager::getTriangleCount() const {
        return this->_triangleCount;
    }

    void RendererManager::render() {
        this->_drawCallCount = 0;
        this->_triangleCount = 0;

        if (this->_activeCamera) {
            const glm::vec3 camPos = this->_activeCamera->getPosition();

            auto distanceSq = [&](const DrawCall& dc) {
                glm::vec3 objPos = glm::vec3(dc.transform[3]);
                glm::vec3 d = objPos - camPos;
                return d.x*d.x + d.y*d.y + d.z*d.z;
            };

            std::sort(this->_opaqueDrawCalls.begin(), this->_opaqueDrawCalls.end(), [&](const DrawCall& a, const DrawCall& b){
                return distanceSq(a) < distanceSq(b);
            });

            std::sort(this->_transparentDrawCalls.begin(), this->_transparentDrawCalls.end(), [&](const DrawCall& a, const DrawCall& b){
                return distanceSq(a) > distanceSq(b);
            });
        }

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
        this->_activeCamera = nullptr;
    }
}
