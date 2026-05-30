#include "Application.hpp"


namespace Voxel {
    Application::Application()
        : _running(false)
        , _lastWindowWidth(0)
        , _lastWindowHeight(0)
        , _physicsAccumulator(0.0)
    {}

    bool Application::initialize() {
        Logger::info("Initializing VoxelEngine...");

        if (!this->_window.create("VoxelEngine", 1280, 720)) {
            Logger::error("Failed to create window");
            return false;
        }

        this->_lastWindowWidth = this->_window.getWidth();
        this->_lastWindowHeight = this->_window.getHeight();

        if (!this->_renderer.initialize(
                this->_window.getNativeWindowHandle(),
                this->_window.getNativeDisplayHandle(),
                this->_lastWindowWidth,
                this->_lastWindowHeight
            )) {
            Logger::error("Failed to initialize renderer");
            return false;
        }

        if (!this->_initializeManagers()) {
            Logger::error("Failed to initialize managers");
            return false;
        }

        this->_running = true;

        Logger::info("VoxelEngine initialized successfully");
        return true;
    }

    bool Application::_initializeManagers() {
        Logger::info("Initializing managers...");

        try {
            this->_serviceLocator.registerManager<TimeManager>();
            this->_serviceLocator.registerManager<InputManager>();
            this->_serviceLocator.registerManager<PhysicsManager>();
            this->_serviceLocator.registerManager<SceneManager>();
            this->_serviceLocator.registerManager<ShaderManager>();
            this->_serviceLocator.registerManager<MaterialManager>();
            this->_serviceLocator.registerManager<MeshManager>();
            this->_serviceLocator.registerManager<CameraManager>();
            this->_serviceLocator.registerManager<RenderManager>();

            this->_serviceLocator.getManager<ShaderManager>().init();
            this->_serviceLocator.getManager<MaterialManager>().init(this->_serviceLocator.getManager<ShaderManager>());

            Mesh::init();

            this->_serviceLocator.getManager<RenderManager>().init(
                this->_serviceLocator.getManager<ShaderManager>(),
                this->_serviceLocator.getManager<MeshManager>(),
                this->_serviceLocator.getManager<CameraManager>()
            );

            Logger::info("Managers initialized successfully");
            return true;
        }
        catch (const std::exception& e) {
            Logger::error("Exception during manager initialization: " + std::string(e.what()));
            return false;
        }
        catch (...) {
            Logger::error("Unknown exception during manager initialization");
            return false;
        }
    }

    bool Application::shutdown() {
        this->_running = false;

        if (!this->_shutdownManagers()) {
            Logger::error("Failed to shutdown managers");
            return false;
        }

        this->_renderer.shutdown();

        Logger::info("VoxelEngine shutdown successfully");
        return true;
    }

    bool Application::_shutdownManagers() {
        try {
            this->_serviceLocator.shutdown();
            Logger::info("Managers shutdown successfully");
            return true;
        }
        catch (const std::exception& e) {
            Logger::error("Exception during manager shutdown: " + std::string(e.what()));
            return false;
        }
        catch (...) {
            Logger::error("Unknown exception during manager shutdown");
            return false;
        }
    }

    void Application::run() {
        while (this->_running) {
            try {
                this->_serviceLocator.getManager<TimeManager>().update();
                double deltaTime = this->_serviceLocator.getManager<TimeManager>().getDeltaTime();

                handleEvents();

                this->_serviceLocator.getManager<InputManager>().pollInput();

                double fixedTimestep = this->_serviceLocator.getManager<PhysicsManager>().getFixedTimestep();
                this->_physicsAccumulator += deltaTime;
                while (_physicsAccumulator >= fixedTimestep) {
                    this->_serviceLocator.getManager<PhysicsManager>().fixedUpdate(fixedTimestep);
                    this->_physicsAccumulator -= fixedTimestep;
                }

                this->_serviceLocator.getManager<SceneManager>().update(deltaTime);

                update();
                render();
            }
            catch (const std::exception& e) {
                Logger::error("Exception in main loop: " + std::string(e.what()));
                this->_running = false;
            }
            catch (...) {
                Logger::error("Unknown exception in main loop");
                this->_running = false;
            }
        }
    }

    void Application::handleEvents() {
        this->_window.pollEvents();

        if (this->_window.shouldClose())
            this->_running = false;

        int currentWidth = this->_window.getWidth();
        int currentHeight = this->_window.getHeight();
        if (currentWidth != this->_lastWindowWidth || currentHeight != this->_lastWindowHeight) {
            this->_lastWindowWidth = currentWidth;
            this->_lastWindowHeight = currentHeight;
            if (this->_renderer.isInitialized())
                this->_renderer.resize(currentWidth, currentHeight);
        }
    }

    void Application::update() {
        this->_serviceLocator.getManager<TimeManager>().debugLogTimeInfo();
    }

    void Application::render() {
        double deltaTime = this->_serviceLocator.getManager<TimeManager>().getDeltaTime();
        this->_serviceLocator.getManager<RenderManager>().render(deltaTime);
        this->_renderer.frame();
    }
}