#include "CameraManager.hpp"

namespace Voxel {
    CameraManager::CameraManager() = default;

    CameraManager::~CameraManager() = default;

    void CameraManager::registerCamera(std::shared_ptr<Camera> cam) {
        if (!cam) return;
        this->_cameras.push_back(cam);
    }

    void CameraManager::unregisterCamera(std::shared_ptr<Camera> cam) {
        if (!cam) return;
        auto it = std::find(this->_cameras.begin(), this->_cameras.end(), cam);
        if (it != this->_cameras.end()) {
            this->_cameras.erase(it);
        }
    }

    void CameraManager::setMainCamera(std::shared_ptr<Camera> cam) {
        this->_mainCamera = cam;
    }

    std::shared_ptr<Camera> CameraManager::getMainCamera() const {
        return this->_mainCamera;
    }

    std::vector<std::shared_ptr<Camera>> CameraManager::getAllCameras() const {
        return this->_cameras;
    }

    bool CameraManager::validateCamera(const Camera& cam) const {
        if (cam.projectionType == ProjectionType::Perspective) {
            if (!(cam.fovDegrees > 0.0f && cam.aspect > 0.0f && cam.zNear > 0.0f && cam.zNear < cam.zFar)) {
                Logger::warning("CameraManager: invalid perspective parameters (fov/aspect/near/far)");
                return false;
            }
        } else {
            if (!(cam.left < cam.right && cam.bottom < cam.top && cam.zNear < cam.zFar)) {
                Logger::warning("CameraManager: invalid orthographic parameters (left/right/bottom/top/near/far)");
                return false;
            }
        }
        return true;
    }
}
