#include "Application.hpp"
#include "../Logger.hpp"

namespace Krio {
    Application::Application()
        : _running(false)
        , _lastWindowWidth(0)
        , _lastWindowHeight(0)
    {}

    bool Application::initialize() {
        Logger::info("Initializing KrioEngine...");

        if (!this->_window.create("KrioEngine", 1280, 720)) {
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

        Logger::info("KrioEngine initialized successfully");
        return true;
    }

    bool Application::_initializeManagers() {
        Logger::info("Initializing managers...");

        this->_serviceLocator.registerManager<TimeManager>();

        Logger::info("Managers initialized successfully");
        return true;
    }

    bool Application::shutdown() {
        this->_running = false;
        this->_renderer.shutdown();

        if (!this->_shutdownManagers()) {
            Logger::error("Failed to shutdown managers");
            return false;
        }

        Logger::info("KrioEngine shutdown successfully");
        return true;
    }

    bool Application::_shutdownManagers() {
        this->_serviceLocator.shutdown();
        Logger::info("Managers shutdown successfully");

        return true;
    }

    void Application::run() {
        while (this->_running) {
            this->_serviceLocator.getManager<TimeManager>().update();

            handleEvents();
            update();
            render();
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
        this->_renderer.frame();
    }
}