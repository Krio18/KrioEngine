#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "Material.hpp"
#include "ShaderManager.hpp"
#include "../Core/ServiceLocator/ServiceLocator.hpp"
#include "../Core/Logger/Logger.hpp"

namespace Krio {
    class MaterialManager {
        public:
            MaterialManager();
            ~MaterialManager();

            void init(ShaderManager& shaderManager);

            std::shared_ptr<Material> create(const std::string& materialName, const std::string& shaderName);
            std::shared_ptr<Material> get(const std::string& materialName);
            std::shared_ptr<Material> instantiate(const std::string& baseName);

        private:
            ShaderManager* _shaderManager;
            std::unordered_map<std::string, std::shared_ptr<Material>> _materials;
            std::unordered_map<std::string, int> _instanceCounters;
    };
}