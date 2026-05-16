#pragma once

#include "../Logger/Logger.hpp"
#include "../../Math/Transform.hpp"
#include "../../Renderer/Shader/Shader.hpp"
#include "../../Renderer/Mesh/Mesh.hpp"
#include "../../Renderer/ShaderManager/ShaderManager.hpp"
#include "../../Renderer/MeshManager/MeshManager.hpp"
#include "../../Renderer/Material/Material.hpp"
#include "../../Renderer/RendererManager/RendererManager.hpp"
#include "../ServiceLocator/ServiceLocator.hpp"

#include <memory>

namespace Krio {
    class RenderManager {
        public:
            RenderManager();
            ~RenderManager();

            void init(ShaderManager& shaderManager, MeshManager& meshManager);
            void render(double deltaTime);
            void submitMesh(const Mesh& mesh, const Material& material, const Transform& transform);

        private:
            RendererManager _rendererManager;
            std::unique_ptr<Material> _material;
            Mesh* _cubeMesh;
            float _angle;
    };
}