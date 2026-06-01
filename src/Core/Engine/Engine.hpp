#pragma once

#include "../../Platform/WindowSDL/WindowSDL.hpp"
#include "../../Platform/InputManager/InputManager.hpp"
#include "../../Renderer/BgfxContext/BgfxContext.hpp"
#include "../ServiceLocator/ServiceLocator.hpp"
#include "../TimeManager/TimeManager.hpp"
#include "../PhysicsManager/PhysicsManager.hpp"
#include "../SceneManager/SceneManager.hpp"
#include "../RenderManager/RenderManager.hpp"
#include "../../Renderer/Mesh/Mesh.hpp"
#include "../../Renderer/ShaderManager/ShaderManager.hpp"
#include "../../Renderer/MaterialManager/MaterialManager.hpp"
#include "../Logger/Logger.hpp"
#include "../../Renderer/MeshManager/MeshManager.hpp"
#include "../../Renderer/CameraManager/CameraManager.hpp"

#include <memory>

namespace Voxel {
    class Engine {
        public:
            Engine();
            virtual ~Engine() = default;

            Engine(const Engine&) = delete;
            Engine& operator=(const Engine&) = delete;

            bool shutdown();
            bool initialize();

            void run();

        protected:
            ServiceLocator& getServiceLocator();
            std::shared_ptr<Camera> getCamera();
            int getWindowWidth() const;
            int getWindowHeight() const;

            virtual void onInit();
            virtual void onUpdate();
            virtual void onShutdown();

            std::shared_ptr<Camera> _camera;

        private:
            WindowSDL _window;
            BgfxContext _renderer;
            bool _running;

            ServiceLocator _serviceLocator;


            int _lastWindowWidth;
            int _lastWindowHeight;
            double _physicsAccumulator;

            bool _initializeManagers();
            bool _shutdownManagers();

            void handleEvents();
            void update();
            void render();
    };
}