#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>
#include <future>

#include <nlohmann/json.hpp>
#include "../Logger/Logger.hpp"

namespace Voxel {
    class Scene;
    class ServiceLocator;

    class SceneManager {
        public:
            SceneManager(ServiceLocator& serviceLocator);
            ~SceneManager();

            void update(double deltaTime);

            Scene* loadScene(const std::string& name);
            void unloadScene(const std::string& name);
            Scene* loadSceneAdditive(const std::string& name);
            Scene* getActiveScene() const;

            Scene* loadSceneFromFile(const std::string& filepath, bool async = false);
            void loadSceneFromFileAsync(const std::string& filepath);

            std::function<void(const std::string&)> onSceneUnload;
            std::function<void(const std::string&)> onSceneLoaded;

        private:
            void activateScene(std::unique_ptr<Scene> scene);
            bool hasPendingSceneLoad() const;

            ServiceLocator& _serviceLocator;
            std::unordered_map<std::string, std::unique_ptr<Scene>> _scenes;
            std::vector<std::string> _activeSceneNames;

            std::future<nlohmann::json> _pendingSceneLoad;
            std::string _pendingSceneFilepath;
    };
}