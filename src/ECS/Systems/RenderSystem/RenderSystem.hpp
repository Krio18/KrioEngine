#pragma once

#include "../../Registry/Registry.hpp"
#include "../../../Renderer/MeshManager/MeshManager.hpp"
#include "../../../Renderer/MaterialManager/MaterialManager.hpp"
#include "../../../Core/RenderManager/RenderManager.hpp"
#include "../../Components/Transform/Transform.hpp"
#include "../../Components/MeshRenderer/MeshRenderer.hpp"
#include "../../Components/Tag/Tag.hpp"

namespace Voxel {
    class RenderSystem {
        public:
            RenderSystem(MeshManager& meshManager, MaterialManager& materialManager, RenderManager& renderManager);
            ~RenderSystem() = default;

            void update(Registry& registry);
        private:
            MeshManager& _meshManager;
            MaterialManager& _materialManager;
            RenderManager& _renderManager;
    };
}
