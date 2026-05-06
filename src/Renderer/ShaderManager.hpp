#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <stdexcept>

#include "../Core/Logger.hpp"
#include "Shader.hpp"

namespace Krio {
    struct ShaderEntry {
        std::unique_ptr<Shader> shader;
        unsigned int usageCount = 0;
    };

    class ShaderManager {
        public:
            ShaderManager();
            ~ShaderManager();

            void load(const std::string& name);
            const Shader& get(const std::string& name);
            void reload(const std::string& name);

        private:
            std::unordered_map<std::string, ShaderEntry> _shaders;
    };
}
