#pragma once

#include <bgfx/bgfx.h>
#include <bx/file.h>
#include <iostream>
#include <fstream>

class Shader {
    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

    private:
        bgfx::ProgramHandle _programHandle;

        bgfx::ShaderHandle _loadShader(const std::string& binShaderPath);
};
