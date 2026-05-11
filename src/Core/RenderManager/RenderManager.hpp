#pragma once

#include "../Logger/Logger.hpp"
#include "../../Renderer/Shader/Shader.hpp"
#include "../../Renderer/Mesh/Mesh.hpp"
#include "../../Math/Transform.hpp"
#include "../../Renderer/ShaderManager/ShaderManager.hpp"
#include "../../Renderer/Material/Material.hpp"
#include "../ServiceLocator/ServiceLocator.hpp"

#include <memory>

namespace Krio {
    class RenderManager {
        public:
            RenderManager();
            ~RenderManager();

            void init(ShaderManager& shaderManager);
            void render(double deltaTime);
            void submitMesh(const Mesh& mesh, const Material& material, const Transform& transform);

        private:
            const Shader* _shader;

            Mesh _cubeMesh;
            float _angle;
    };
}