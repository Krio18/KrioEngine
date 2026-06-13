#include "SceneManager.hpp"
#include "../../Scene/Scene.hpp"
#include "../../Scene/SceneSerializer.hpp"
#include "../ServiceLocator/ServiceLocator.hpp"

#include <algorithm>
#include <chrono>

namespace Voxel {
    SceneManager::SceneManager(ServiceLocator& serviceLocator) : _serviceLocator(serviceLocator) {
        Logger::info("SceneManager created");
    }

    SceneManager::~SceneManager() {
        if (this->_pendingSceneLoad.valid()) {
            this->_pendingSceneLoad.wait();
        }
    }

    void SceneManager::update(double deltaTime) {
        if (this->_pendingSceneLoad.valid()) {
            if (this->_pendingSceneLoad.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
                nlohmann::json root = this->_pendingSceneLoad.get();
                if (!root.is_null()) {
                    auto scene = SceneSerializer::deserialize(root,
                        this->_serviceLocator.getManager<MeshManager>(),
                        this->_serviceLocator.getManager<MaterialManager>(),
                        this->_serviceLocator.getManager<RenderManager>(),
                        this->_serviceLocator.getManager<CameraManager>()
                    );
                    if (scene) {
                        activateScene(std::move(scene));
                    } else {
                        Logger::warning("SceneManager: asynchronous scene construction failed for '" + this->_pendingSceneFilepath + "'");
                    }
                } else {
                    Logger::warning("SceneManager: asynchronous scene load failed for '" + this->_pendingSceneFilepath + "'");
                }
                this->_pendingSceneFilepath.clear();
            }
        }

        for (const auto& sceneName : this->_activeSceneNames) {
            auto it = this->_scenes.find(sceneName);
            if (it != this->_scenes.end() && it->second) {
                it->second->update(deltaTime);
            }
        }
    }

    Scene* SceneManager::loadScene(const std::string& name) {
        if (name.empty()) {
            return nullptr;
        }

        std::vector<std::string> activeScenes = this->_activeSceneNames;
        for (const auto& activeScene : activeScenes) {
            if (activeScene != name) {
                unloadScene(activeScene);
            }
        }

        auto it = this->_scenes.find(name);
        if (it == this->_scenes.end()) {
            auto scene = std::make_unique<Scene>(name,
                this->_serviceLocator.getManager<MeshManager>(),
                this->_serviceLocator.getManager<MaterialManager>(),
                this->_serviceLocator.getManager<RenderManager>(),
                this->_serviceLocator.getManager<CameraManager>()
            );
            it = this->_scenes.emplace(name, std::move(scene)).first;
        }

        this->_activeSceneNames.clear();
        this->_activeSceneNames.push_back(name);

        if (this->onSceneLoaded) {
            this->onSceneLoaded(name);
        }

        return it->second.get();
    }

    Scene* SceneManager::loadSceneFromFile(const std::string& filepath, bool async) {
        if (filepath.empty()) {
            return nullptr;
        }

        if (async) {
            loadSceneFromFileAsync(filepath);
            return nullptr;
        }

        auto scene = SceneSerializer::deserialize(filepath,
            this->_serviceLocator.getManager<MeshManager>(),
            this->_serviceLocator.getManager<MaterialManager>(),
            this->_serviceLocator.getManager<RenderManager>(),
            this->_serviceLocator.getManager<CameraManager>()
        );

        if (!scene) {
            Logger::warning("SceneManager: failed to deserialize scene from file '" + filepath + "'");
            return nullptr;
        }

        const std::string sceneName = scene->getName();
        activateScene(std::move(scene));
        return getActiveScene();
    }

    void SceneManager::loadSceneFromFileAsync(const std::string& filepath) {
        if (filepath.empty()) {
            return;
        }

        if (this->_pendingSceneLoad.valid()) {
            Logger::warning("SceneManager: scene load already pending for '" + this->_pendingSceneFilepath + "'");
            return;
        }

        this->_pendingSceneFilepath = filepath;

        this->_pendingSceneLoad = std::async(std::launch::async, [filepath]() {
            nlohmann::json root;
            if (!SceneSerializer::loadJsonFile(filepath, root)) {
                return nlohmann::json();
            }
            return root;
        });
    }

    void SceneManager::activateScene(std::unique_ptr<Scene> scene) {
        if (!scene) {
            return;
        }

        const std::string sceneName = scene->getName();
        std::vector<std::string> activeScenes = this->_activeSceneNames;

        for (const auto& activeScene : activeScenes) {
            if (activeScene == sceneName) {
                continue;
            }

            unloadScene(activeScene);
        }

        auto existingSceneIt = this->_scenes.find(sceneName);
        if (existingSceneIt != this->_scenes.end()) {
            unloadScene(sceneName);
        }

        this->_scenes.emplace(sceneName, std::move(scene));
        this->_activeSceneNames.clear();
        this->_activeSceneNames.push_back(sceneName);

        if (this->onSceneLoaded) {
            this->onSceneLoaded(sceneName);
        }
    }

    bool SceneManager::hasPendingSceneLoad() const {
        return this->_pendingSceneLoad.valid();
    }

    Scene* SceneManager::loadSceneAdditive(const std::string& name) {
        if (name.empty()) {
            return nullptr;
        }

        auto it = this->_scenes.find(name);
        if (it == this->_scenes.end()) {
            auto scene = std::make_unique<Scene>(name,
                this->_serviceLocator.getManager<MeshManager>(),
                this->_serviceLocator.getManager<MaterialManager>(),
                this->_serviceLocator.getManager<RenderManager>(),
                this->_serviceLocator.getManager<CameraManager>()
            );
            it = this->_scenes.emplace(name, std::move(scene)).first;
        }

        if (std::find(this->_activeSceneNames.begin(), this->_activeSceneNames.end(), name) == this->_activeSceneNames.end()) {
            this->_activeSceneNames.push_back(name);
            if (this->onSceneLoaded) {
                this->onSceneLoaded(name);
            }
        }

        return it->second.get();
    }

    void SceneManager::unloadScene(const std::string& name) {
        if (this->onSceneUnload) {
            this->onSceneUnload(name);
        }

        auto activeIt = std::find(this->_activeSceneNames.begin(), this->_activeSceneNames.end(), name);
        if (activeIt != this->_activeSceneNames.end()) {
            this->_activeSceneNames.erase(activeIt);
        }

        this->_scenes.erase(name);
    }

    Scene* SceneManager::getActiveScene() const {
        if (this->_activeSceneNames.empty()) {
            return nullptr;
        }

        const std::string& activeName = this->_activeSceneNames.back();
        auto it = this->_scenes.find(activeName);
        return it != this->_scenes.end() ? it->second.get() : nullptr;
    }
}
