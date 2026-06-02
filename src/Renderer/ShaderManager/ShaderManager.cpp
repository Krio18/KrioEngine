#include "ShaderManager.hpp"

namespace Voxel {
    ShaderManager::ShaderManager() : _errorProgramHandle(BGFX_INVALID_HANDLE), _shaderDirectory("build/shaders/spirv") {}

    void ShaderManager::init() {
        this->load("error");

        auto it = this->_shaders.find("error");
        if (it != this->_shaders.end()) {
            this->_errorProgramHandle = it->second.shader->getProgramHandle();
            Logger::info("Fallback error shader loaded successfully");
        } else {
            Logger::error("Critical: Error shader ('error') not found in " + this->_shaderDirectory.string());
        }
    }

    void ShaderManager::load(const std::string& name) {
        if (this->_shaders.contains(name)) {
            Logger::warning("this shader already exists: " + name);
            return;
        }

        std::string vertexPath = (_shaderDirectory / ("v_" + name + ".sc.bin")).string();
        std::string fragmentPath = (_shaderDirectory / ("f_" + name + ".sc.bin")).string();

        std::unique_ptr<Voxel::Shader> shader = std::make_unique<Shader>(vertexPath, fragmentPath);

        if (!bgfx::isValid(shader->getProgramHandle())) {
            Logger::error("invalid Shader : " + name);
            return;
        }

        this->_shaders.emplace(name, ShaderEntry{std::move(shader)});
    }

    const Shader& ShaderManager::get(const std::string& name) {
        auto it = this->_shaders.find(name);
        if (it != this->_shaders.end()) {
            return *(it->second.shader);
        } else {
            Logger::warning("Shader '" + name + "' not found. Using fallback 'error' shader.");

            auto errorIt = this->_shaders.find("error");
            if (errorIt != this->_shaders.end()) {
                return *(errorIt->second.shader);
            }
            throw std::runtime_error("Shader not found and error shader is not loaded: " + name);
        }
    }

    bgfx::ProgramHandle ShaderManager::getHandle(const std::string& name) {
        auto it = this->_shaders.find(name);

        if (it != this->_shaders.end()) {
            return it->second.shader->getProgramHandle();
        }

        Logger::warning("Shader '" + name + "' not found. Using fallback 'error' shader.");
        return this->_errorProgramHandle;
    }

    void ShaderManager::reload(const std::string& name) {
        if (this->_shaders.contains(name)) {
            this->_shaders.erase(name);
            load(name);
            return;
        }
        Logger::warning("Shader not found: " + name);
        return;
    }
}