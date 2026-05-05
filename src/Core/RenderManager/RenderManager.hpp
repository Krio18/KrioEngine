#pragma once

#include "../Logger.hpp"
#include "../../Renderer/Shader.hpp"
#include "../../Renderer/Mesh.hpp"
#include "../../Math/Transform.hpp"

#include <memory>

namespace Krio {
    class RenderManager {
        public:
            RenderManager();
            ~RenderManager();

            void init();
            void render(double deltaTime);

        private:
            std::unique_ptr<Shader> _shader;
            Mesh _cubeMesh;
            float _angle;
    };
}