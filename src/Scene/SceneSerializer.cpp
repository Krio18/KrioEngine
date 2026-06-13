#include "SceneSerializer.hpp"
#include "Scene.hpp"
#include "../Core/Logger/Logger.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

namespace Voxel {
    namespace {
        constexpr int kSceneFileVersion = 1;
    }

    bool SceneSerializer::serialize(const Scene& scene, const std::string& filepath) {
        nlohmann::json root;
        root["version"] = kSceneFileVersion;
        root["sceneName"] = scene._name;

        root["settings"] = {
            {"ambientLightColor", vec3ToJson(scene.ambientLightColor)},
            {"fogEnabled", scene.fogEnabled},
            {"skyboxName", scene.skyboxName}
        };

        root["entities"] = nlohmann::json::array();
        root["hierarchy"] = nlohmann::json::array();

        scene._registry.eachEntity([&](entt::entity entityId) {
            Entity entity(entityId, const_cast<Registry*>(&scene._registry));
            nlohmann::json entityJson;
            entityJson["id"] = static_cast<uint32_t>(entityId);
            entityJson["components"] = nlohmann::json::object();

            if (scene._registry.hasComponent<Tag>(entity)) {
                const Tag* tag = scene._registry.getComponent<Tag>(entity);
                if (tag) {
                    nlohmann::json tagJson;
                    tagJson["entityName"] = tag->entityName;
                    tagJson["enableEntity"] = tag->enableEntity;
                    tagJson["layer"] = tag->layer;
                    tagJson["tag"] = tag->tag;
                    entityJson["components"]["Tag"] = std::move(tagJson);
                }
            }

            if (scene._registry.hasComponent<Transform>(entity)) {
                const Transform* transform = scene._registry.getComponent<Transform>(entity);
                if (transform) {
                    nlohmann::json transformJson;
                    transformJson["position"] = vec3ToJson(transform->getPosition());
                    transformJson["rotation"] = quatToJson(transform->getRotation());
                    transformJson["scale"] = vec3ToJson(transform->getScale());
                    entityJson["components"]["Transform"] = std::move(transformJson);
                }
            }

            if (scene._registry.hasComponent<MeshRenderer>(entity)) {
                const MeshRenderer* meshRenderer = scene._registry.getComponent<MeshRenderer>(entity);
                if (meshRenderer) {
                    nlohmann::json rendererJson;
                    rendererJson["meshName"] = meshRenderer->meshName;
                    rendererJson["materialName"] = meshRenderer->materialName;
                    rendererJson["castShadows"] = meshRenderer->castShadows;
                    rendererJson["receiveShadows"] = meshRenderer->receiveShadows;
                    rendererJson["renderingLayerMask"] = meshRenderer->renderingLayerMask;
                    rendererJson["boundsMin"] = vec3ToJson(meshRenderer->boundsMin);
                    rendererJson["boundsMax"] = vec3ToJson(meshRenderer->boundsMax);
                    entityJson["components"]["MeshRenderer"] = std::move(rendererJson);
                }
            }

            if (scene._registry.hasComponent<Camera>(entity)) {
                const Camera* camera = scene._registry.getComponent<Camera>(entity);
                if (camera) {
                    nlohmann::json cameraJson;
                    cameraJson["projectionType"] = projectionTypeToString(camera->projectionType);
                    cameraJson["fovDegrees"] = camera->fovDegrees;
                    cameraJson["aspect"] = camera->aspect;
                    cameraJson["zNear"] = camera->zNear;
                    cameraJson["zFar"] = camera->zFar;
                    cameraJson["left"] = camera->left;
                    cameraJson["right"] = camera->right;
                    cameraJson["bottom"] = camera->bottom;
                    cameraJson["top"] = camera->top;
                    cameraJson["priority"] = camera->priority;
                    entityJson["components"]["Camera"] = std::move(cameraJson);
                }
            }

            root["entities"].push_back(std::move(entityJson));

            auto parentIt = scene._parentMap.find(entityId);
            if (parentIt != scene._parentMap.end()) {
                root["hierarchy"].push_back({
                    {"entity", static_cast<uint32_t>(entityId)},
                    {"parent", static_cast<uint32_t>(parentIt->second)}
                });
            }
        });

        std::ofstream outfile(filepath);
        if (!outfile.is_open()) {
            Logger::error("Failed to open scene file for writing: " + filepath);
            return false;
        }

        outfile << root.dump(4);
        return true;
    }

    bool SceneSerializer::loadJsonFile(const std::string& filepath, nlohmann::json& outJson) {
        std::ifstream infile(filepath);
        if (!infile.is_open()) {
            Logger::error("Failed to open scene file for reading: " + filepath);
            return false;
        }

        infile >> outJson;
        return true;
    }

    std::unique_ptr<Scene> SceneSerializer::deserialize(const std::string& filepath,
                                                         MeshManager& meshManager,
                                                         MaterialManager& materialManager,
                                                         RenderManager& renderManager,
                                                         CameraManager& cameraManager) {
        nlohmann::json root;
        if (!loadJsonFile(filepath, root)) {
            return nullptr;
        }

        return deserialize(root, meshManager, materialManager, renderManager, cameraManager);
    }

    std::unique_ptr<Scene> SceneSerializer::deserialize(const nlohmann::json& root,
                                                         MeshManager& meshManager,
                                                         MaterialManager& materialManager,
                                                         RenderManager& renderManager,
                                                         CameraManager& cameraManager) {
        if (getSceneVersion(root) != kSceneFileVersion) {
            Logger::error("Unsupported scene version in provided JSON");
            return nullptr;
        }

        const std::string sceneName = root.value("sceneName", "UntitledScene");
        auto scene = std::make_unique<Scene>(sceneName, meshManager, materialManager, renderManager, cameraManager);

        if (root.contains("settings")) {
            const auto& settingsJson = root["settings"];
            if (settingsJson.contains("ambientLightColor")) {
                scene->ambientLightColor = jsonToVec3(settingsJson["ambientLightColor"]);
            }
            scene->fogEnabled = settingsJson.value("fogEnabled", false);
            scene->skyboxName = settingsJson.value("skyboxName", std::string());
        }

        std::unordered_map<uint32_t, Entity> entityMap;
        if (root.contains("entities") && root["entities"].is_array()) {
            for (const auto& entityJson : root["entities"]) {
                uint32_t entityId = entityJson.value("id", 0u);
                std::string entityName = "Entity" + std::to_string(entityId);
                if (entityJson.contains("components") && entityJson["components"].contains("Tag")) {
                    const auto& tagJson = entityJson["components"]["Tag"];
                    entityName = tagJson.value("entityName", entityName);
                }

                Entity entity = scene->createEntity(entityName);
                entityMap.insert_or_assign(entityId, entity);

                if (entityJson.contains("components")) {
                    const auto& componentsJson = entityJson["components"];
                    if (componentsJson.contains("Tag")) {
                        const auto& tagJson = componentsJson["Tag"];
                        Tag* tag = scene->_registry.getComponent<Tag>(entity);
                        if (tag) {
                            tag->entityName = tagJson.value("entityName", tag->entityName);
                            tag->enableEntity = tagJson.value("enableEntity", tag->enableEntity);
                            tag->layer = tagJson.value("layer", tag->layer);
                            tag->tag = tagJson.value("tag", tag->tag);
                        }
                    }

                    if (componentsJson.contains("Transform")) {
                        const auto& transformJson = componentsJson["Transform"];
                        Transform transform;
                        if (transformJson.contains("position")) {
                            transform.setPosition(jsonToVec3(transformJson["position"]));
                        }
                        if (transformJson.contains("rotation")) {
                            transform.setRotation(jsonToQuat(transformJson["rotation"]));
                        }
                        if (transformJson.contains("scale")) {
                            transform.setScale(jsonToVec3(transformJson["scale"]));
                        }
                        scene->_registry.addComponent<Transform>(entity, transform);
                    }

                    if (componentsJson.contains("MeshRenderer")) {
                        const auto& rendererJson = componentsJson["MeshRenderer"];
                        MeshRenderer renderer;
                        renderer.meshName = rendererJson.value("meshName", std::string());
                        renderer.materialName = rendererJson.value("materialName", std::string());
                        renderer.castShadows = rendererJson.value("castShadows", renderer.castShadows);
                        renderer.receiveShadows = rendererJson.value("receiveShadows", renderer.receiveShadows);
                        renderer.renderingLayerMask = rendererJson.value("renderingLayerMask", renderer.renderingLayerMask);
                        if (rendererJson.contains("boundsMin")) {
                            renderer.boundsMin = jsonToVec3(rendererJson["boundsMin"]);
                        }
                        if (rendererJson.contains("boundsMax")) {
                            renderer.boundsMax = jsonToVec3(rendererJson["boundsMax"]);
                        }
                        scene->_registry.addComponent<MeshRenderer>(entity, renderer);
                    }

                    if (componentsJson.contains("Camera")) {
                        const auto& cameraJson = componentsJson["Camera"];
                        Camera camera;
                        camera.projectionType = stringToProjectionType(cameraJson.value("projectionType", projectionTypeToString(camera.projectionType)));
                        camera.fovDegrees = cameraJson.value("fovDegrees", camera.fovDegrees);
                        camera.aspect = cameraJson.value("aspect", camera.aspect);
                        camera.zNear = cameraJson.value("zNear", camera.zNear);
                        camera.zFar = cameraJson.value("zFar", camera.zFar);
                        camera.left = cameraJson.value("left", camera.left);
                        camera.right = cameraJson.value("right", camera.right);
                        camera.bottom = cameraJson.value("bottom", camera.bottom);
                        camera.top = cameraJson.value("top", camera.top);
                        camera.priority = cameraJson.value("priority", camera.priority);
                        scene->_registry.addComponent<Camera>(entity, camera);
                    }
                }
            }
        }

        if (root.contains("hierarchy") && root["hierarchy"].is_array()) {
            for (const auto& hierarchyJson : root["hierarchy"]) {
                uint32_t entityId = hierarchyJson.value("entity", 0u);
                uint32_t parentId = hierarchyJson.value("parent", 0u);
                if (entityId == 0u || parentId == 0u) {
                    continue;
                }

                auto entityIt = entityMap.find(entityId);
                auto parentIt = entityMap.find(parentId);
                if (entityIt != entityMap.end() && parentIt != entityMap.end()) {
                    scene->setParent(entityIt->second, parentIt->second);
                }
            }
        }

        return scene;
    }

    int SceneSerializer::getSceneVersion(const nlohmann::json& jsonData) {
        return jsonData.value("version", 0);
    }

    std::string SceneSerializer::projectionTypeToString(ProjectionType type) {
        switch (type) {
            case ProjectionType::Orthographic:
                return "Orthographic";
            default:
                return "Perspective";
        }
    }

    ProjectionType SceneSerializer::stringToProjectionType(const std::string& typeString) {
        if (typeString == "Orthographic") {
            return ProjectionType::Orthographic;
        }

        return ProjectionType::Perspective;
    }

    nlohmann::json SceneSerializer::vec3ToJson(const glm::vec3& value) {
        return nlohmann::json::array({value.x, value.y, value.z});
    }

    glm::vec3 SceneSerializer::jsonToVec3(const nlohmann::json& jsonValue) {
        if (!jsonValue.is_array() || jsonValue.size() != 3) {
            return glm::vec3(0.0f);
        }
        return glm::vec3(jsonValue[0].get<float>(), jsonValue[1].get<float>(), jsonValue[2].get<float>());
    }

    nlohmann::json SceneSerializer::quatToJson(const glm::quat& value) {
        return nlohmann::json::array({value.x, value.y, value.z, value.w});
    }

    glm::quat SceneSerializer::jsonToQuat(const nlohmann::json& jsonValue) {
        if (!jsonValue.is_array() || jsonValue.size() != 4) {
            return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        }
        return glm::quat(jsonValue[3].get<float>(), jsonValue[0].get<float>(), jsonValue[1].get<float>(), jsonValue[2].get<float>());
    }
}
