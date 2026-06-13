#pragma once

#include "Scene.hpp"
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

namespace Voxel {
    class SceneSerializer {
        public:
            static bool serialize(const Scene& scene, const std::string& filepath);
            static std::unique_ptr<Scene> deserialize(const std::string& filepath,
                                                     MeshManager& meshManager,
                                                     MaterialManager& materialManager,
                                                     RenderManager& renderManager,
                                                     CameraManager& cameraManager);
            static std::unique_ptr<Scene> deserialize(const nlohmann::json& jsonData,
                                                     MeshManager& meshManager,
                                                     MaterialManager& materialManager,
                                                     RenderManager& renderManager,
                                                     CameraManager& cameraManager);
            static bool loadJsonFile(const std::string& filepath, nlohmann::json& outJson);

        private:
            static int getSceneVersion(const nlohmann::json& jsonData);
            static std::string projectionTypeToString(ProjectionType type);
            static ProjectionType stringToProjectionType(const std::string& typeString);

            static nlohmann::json vec3ToJson(const glm::vec3& value);
            static glm::vec3 jsonToVec3(const nlohmann::json& jsonValue);
            static nlohmann::json quatToJson(const glm::quat& value);
            static glm::quat jsonToQuat(const nlohmann::json& jsonValue);
    };
}
