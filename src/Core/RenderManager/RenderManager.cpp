#include "RenderManager.hpp"

namespace Voxel {
    RenderManager::RenderManager() {
        this->_angle = 0.0f;
        this->_cubeMesh = nullptr;

        Logger::info("RenderManager created");
    }

    RenderManager::~RenderManager() {
        Logger::info("RenderManager destroyed");
    }

    void RenderManager::init(ShaderManager& shaderManager, MeshManager& meshManager, CameraManager& cameraManager) {
        shaderManager.load("simple");
        const Shader& shader = shaderManager.get("simple");

        meshManager.load("cube");
        this->_cubeMesh = &meshManager.get("cube");

        this->_material = std::make_unique<Material>(shader.getProgramHandle(), "simple");
        this->_cameraManager = &cameraManager;
    }

    void RenderManager::render( double deltaTime) {
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::touch(0);

        this->_angle += 1.0f * deltaTime;
        glm::mat4 model = Transform::createModelMatrix(glm::vec3(0.0f), glm::angleAxis(this->_angle, glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f));
        glm::mat4 view;
        glm::mat4 proj;
        const Voxel::Camera* mainCamPtr = nullptr;

        if (this->_cameraManager) {
            auto mainCam = this->_cameraManager->getMainCamera();
            if (mainCam) {
                view = mainCam->getViewMatrix();
                proj = mainCam->getProjectionMatrix();
                mainCamPtr = mainCam.get();
            }
        }

        if (!mainCamPtr) {
            view = Transform::createViewMatrix(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            proj = Transform::createPerspectiveMatrix(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        }

        bgfx::setViewTransform(0, &view, &proj);

        this->_rendererManager.submitCamera(mainCamPtr);
        this->_rendererManager.submitMesh(this->_cubeMesh, this->_material.get(), model);
        this->_rendererManager.render();
    }

    void RenderManager::submitMesh(const Mesh& mesh, const Material& material, const Transform& transform) {
        this->_rendererManager.submitMesh(&mesh, &material, glm::mat4(1.0f));
    }
}
