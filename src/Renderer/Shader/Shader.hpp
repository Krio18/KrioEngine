#pragma once

#include "../../Core/Logger/Logger.hpp"

#include <bgfx/bgfx.h>
#include <bx/file.h>
#include <iostream>
#include <fstream>

namespace Voxel {
    class Shader {
        public:
            Shader(const std::string& vertexPath, const std::string& fragmentPath);
            ~Shader();

            bgfx::ProgramHandle getProgramHandle() const;

            Shader(const Shader&) = delete;
            Shader& operator=(const Shader&) = delete;

        private:
            bgfx::ProgramHandle _programHandle;

            bgfx::ShaderHandle _loadShader(const std::string& binShaderPath);
    };
};