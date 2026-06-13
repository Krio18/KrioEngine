#pragma once

#include "../src/Core/Engine/Engine.hpp"
#include "../src/Scene/Scene.hpp"
#include "../src/ECS/Entity/Entity.hpp"
#include <vector>

namespace Voxel {
    class Sandbox : public Engine {
        public:
            Sandbox();
            ~Sandbox() = default;

            Sandbox(const Sandbox&) = delete;
            Sandbox& operator=(const Sandbox&) = delete;

        protected:
            void onInit() override;
            void onUpdate() override;
            void onShutdown() override;

        private:
            float  _time  = 0.0f;
            Scene* _scene = nullptr;

            Entity _cameraEntity{ entt::null, nullptr };

            std::vector<Entity> _spire;      // central gold tower (5 cubes)
            std::vector<Entity> _ring1;      // inner  cyan    ring (14 cubes)
            std::vector<Entity> _ring2;      // middle magenta ring (20 cubes)
            std::vector<Entity> _ring3;      // outer  orange  ring (28 cubes)
            std::vector<Entity> _wanderers;  // white 3D Lissajous wanderers (7)
            std::vector<Entity> _grid;       // ground wave grid (7×7 = 49)
    };
}
