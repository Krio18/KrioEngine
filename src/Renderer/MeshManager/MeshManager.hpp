#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <stdexcept>

#include "../Mesh/Mesh.hpp"
#include "../../Core/Logger/Logger.hpp"

namespace Voxel {
    struct MeshEntry {
        std::shared_ptr<Mesh> mesh;
        int refCount = 0;
    };

    class MeshManager {
        public:
            MeshManager() = default;

            void load(const std::string& path);
            std::shared_ptr<Mesh> get(const std::string& name);
            void unload(const std::string& name);

        private:
            std::unordered_map<std::string, MeshEntry> _meshMap;
    };
}
