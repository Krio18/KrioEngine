#include "Sandbox.hpp"

namespace Voxel {
    Sandbox::Sandbox() : Engine(), _targetPosition(0.0f), _targetAngle(0.0f), _mainAngle(0.0f) {}

    void Sandbox::onInit() {
        Logger::info("Sandbox::onInit() - Initializing sandbox logic");

        int width = getWindowWidth();
        int height = getWindowHeight();

        this->_camera = std::make_shared<Camera>();
        this->_camera->position = glm::vec3(0.0f, 2.0f, 5.0f);
        this->_camera->aspect = static_cast<float>(width) / static_cast<float>(height);

        getServiceLocator().getManager<CameraManager>().registerCamera(this->_camera);
        getServiceLocator().getManager<CameraManager>().setMainCamera(this->_camera);

        this->_cameraController = std::make_unique<FollowCameraController>(*(this->_camera.get()));
        this->_cameraController->setTarget(&this->_targetPosition);
        this->_cameraController->setOffset(glm::vec3(0.0f, 2.0f, 5.0f));
        this->_cameraController->setDamping(3.0f);

        getServiceLocator().getManager<ShaderManager>().load("simple");
        getServiceLocator().getManager<MeshManager>().load("cube");
        getServiceLocator().getManager<MaterialManager>().create("simple", "simple");

        this->_cubeMesh = &getServiceLocator().getManager<MeshManager>().get("cube");
        this->_material = getServiceLocator().getManager<MaterialManager>().get("simple");

        Logger::info("Sandbox::onInit() - Sandbox logic initialized successfully");
    }

    void Sandbox::onUpdate() {
        float deltaTime = static_cast<float>(getServiceLocator().getManager<TimeManager>().getDeltaTime());

        // La cible tourne autour de l'origine — simule un objet en mouvement
        this->_targetAngle += 1.0f * deltaTime;
        this->_targetPosition.x = 3.0f * std::cos(this->_targetAngle);
        this->_targetPosition.z = 3.0f * std::sin(this->_targetAngle);

        if (this->_cameraController) {
            this->_cameraController->update(deltaTime);
        }

        if (this->_cubeMesh && this->_material) {
            // Cube principal qui tourne sur lui-même
            this->_mainAngle += 1.0f * deltaTime;
            glm::mat4 mainModel = Transform::createModelMatrix(
                glm::vec3(0.0f),
                glm::angleAxis(this->_mainAngle, glm::vec3(0.0f, 1.0f, 0.0f)),
                glm::vec3(1.0f)
            );
            getServiceLocator().getManager<RenderManager>().submitMesh(*this->_cubeMesh, *this->_material, mainModel);

            // Petit cube à la position de la cible
            glm::mat4 targetModel = Transform::createModelMatrix(
                this->_targetPosition,
                glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                glm::vec3(0.3f)
            );
            getServiceLocator().getManager<RenderManager>().submitMesh(*this->_cubeMesh, *this->_material, targetModel);
        }
    }

    void Sandbox::onShutdown() {
        Logger::info("Sandbox::onShutdown() - Cleaning up sandbox logic");
    }
}
