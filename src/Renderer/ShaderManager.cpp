#include "ShaderManager.hpp"

namespace Krio {
    ShaderManager::ShaderManager() {}
    ShaderManager::~ShaderManager() {}

    void ShaderManager::load(const std::string& name) {
        if (this->_shaders.contains(name)) {
            Logger::warning("this shader already exists: " + name);
            return;
        }

        std::string vertexPath = "build/shaders/spirv/v_" + name + ".sc.bin";
        std::string fragmentPath = "build/shaders/spirv/f_" + name + ".sc.bin";

        std::unique_ptr<Krio::Shader> shader = std::make_unique<Shader>(vertexPath, fragmentPath);

        if (!bgfx::isValid(shader->getProgramHandle())) {
            Logger::error("invalid Shader : " + name);
            return;
        }

        this->_shaders.insert({name, ShaderEntry{std::move(shader), 1}});
    }

    const Shader& ShaderManager::get(const std::string& name) {
        auto it = this->_shaders.find(name);
        if (it != this->_shaders.end()) {
            it->second.usageCount++;
            return *(it->second.shader);
        } else {
            Logger::error("Shader not found: " + name);
            throw std::runtime_error("Shader not found: " + name);
        }
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