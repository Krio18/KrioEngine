#pragma once

#include "../Logger.hpp"
#include "../../Renderer/Shader.hpp"
#include "../../Renderer/Mesh.hpp"
#include "../../Math/Transform.hpp"
#include "../../Renderer/ShaderManager.hpp"
#include "../ServiceLocator/ServiceLocator.hpp"

#include <memory>

namespace Krio {
    class RenderManager {
        public:
            RenderManager();
            ~RenderManager();

            void init(ShaderManager& shaderManager);
            void render(double deltaTime);

        private:
            const Shader* _shader;

            Mesh _cubeMesh;
            float _angle;
    };
}