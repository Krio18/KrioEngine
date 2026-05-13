#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "../Mesh/Mesh.hpp"
#include "../../Core/Logger/Logger.hpp"

namespace Krio {
    struct MeshEntry {
        Mesh mesh;
        int refCount = 0;
    };

    class MeshManager {
        public:
            MeshManager();
            ~MeshManager();

            void load(const std::string& path);
            Mesh& get(const std::string& name);
            void unload(const std::string& name);

        private:
            std::unordered_map<std::string, MeshEntry> _meshMap;
    };
}
