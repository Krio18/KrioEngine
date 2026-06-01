#pragma once

#include "../Logger/Logger.hpp"
#include "../../Math/Transform.hpp"
#include "../../Renderer/Shader/Shader.hpp"
#include "../../Renderer/Mesh/Mesh.hpp"
#include "../../Renderer/ShaderManager/ShaderManager.hpp"
#include "../../Renderer/MeshManager/MeshManager.hpp"
#include "../../Renderer/Material/Material.hpp"
#include "../../Renderer/RendererManager/RendererManager.hpp"
#include "../../Renderer/CameraManager/CameraManager.hpp"

#include <glm/glm.hpp>
#include <memory>

namespace Voxel {
    class CameraManager;
    class RenderManager {
        public:
            RenderManager();
            ~RenderManager();

            void init(ShaderManager& shaderManager, MeshManager& meshManager, CameraManager& cameraManager);
            void render(double deltaTime);
            void submitMesh(const Mesh& mesh, const Material& material, const glm::mat4& transform);

        private:
            RendererManager _rendererManager;
            CameraManager* _cameraManager = nullptr;
    };
}