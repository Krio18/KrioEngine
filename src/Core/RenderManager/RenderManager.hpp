#pragma once

#include "../Logger.hpp"
#include "../../Renderer/Shader.hpp"
#include "../../Renderer/Mesh.hpp"

#include <memory>

namespace Krio {
    class RenderManager {
        public:
            RenderManager();
            ~RenderManager() = default;

            void init();
            void render();

        private:
            std::unique_ptr<Shader> _shader;
            Mesh _triangleMesh;
    };
}