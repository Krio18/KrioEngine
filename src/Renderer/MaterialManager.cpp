#include "MaterialManager.hpp"

namespace Krio {
    MaterialManager::MaterialManager() : _shaderManager(nullptr) {}
    MaterialManager::~MaterialManager() {
        for (auto& [name, material] : this->_materials) {
            material.reset();
        }
    }

    void MaterialManager::init(ShaderManager& shaderManager) {
        this->_shaderManager = &shaderManager;
    }

    std::shared_ptr<Material> MaterialManager::create(const std::string& materialName, const std::string& shaderName) {
        if (_materials.contains(materialName)) {
            Logger::warning("Material already exists: " + materialName);
            return _materials[materialName];
        }

        bgfx::ProgramHandle handle = this->_shaderManager->getHandle(shaderName);

        this->_materials[materialName] = std::make_shared<Material>(handle, shaderName);

        return this->_materials[materialName];
    }

    std::shared_ptr<Material> MaterialManager::get(const std::string& materialName) {
        return this->_materials[materialName];
    }

    std::shared_ptr<Material> MaterialManager::instantiate(const std::string& baseName) {
        auto it = this->_materials.find(baseName);
        if (it != this->_materials.end()) {
            int count = ++_instanceCounters[baseName];
            std::string newName = baseName + "_copy_" + std::to_string(count);

            std::shared_ptr<Material> instance = std::make_shared<Material>(*(it->second));
            this->_materials.insert({newName, instance});

            return instance;
        }

        Logger::error("Material not found: " + baseName);
        return nullptr;
    }

}