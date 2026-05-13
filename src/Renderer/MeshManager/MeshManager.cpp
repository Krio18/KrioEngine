#include "MeshManager.hpp"

namespace Krio {
    void MeshManager::load(const std::string& path) {
        auto it = this->_meshMap.find(path);
        if (it != this->_meshMap.end()) {
            it->second.refCount++;
            Logger::info("Mesh already loaded, incremented refCount: " + path);
            return;
        }

        MeshEntry entry;
        if (path == "triangle") {
            entry.mesh = Mesh::createTriangle();
            entry.refCount = 1;
            this->_meshMap.emplace(path, std::move(entry));
            Logger::info("Loaded primitive mesh: " + path);
            return;
        }

        if (path == "quad") {
            entry.mesh = Mesh::createQuad();
            entry.refCount = 1;
            this->_meshMap.emplace(path, std::move(entry));
            Logger::info("Loaded primitive mesh: " + path);
            return;
        }

        if (path == "cube") {
            entry.mesh = Mesh::createCube();
            entry.refCount = 1;
            this->_meshMap.emplace(path, std::move(entry));
            Logger::info("Loaded primitive mesh: " + path);
            return;
        }

        std::ifstream file(path);
        if (!file.is_open()) {
            Logger::error("Could not open Mesh file: " + path);
            return;
        }

        Logger::warning("Mesh file loading is not implemented yet: " + path);
        file.close();
    }

    Mesh& MeshManager::get(const std::string& name) {
        auto it = this->_meshMap.find(name);
        if (it == this->_meshMap.end()) {
            Logger::error("Mesh not found: " + name);
            throw std::runtime_error("Mesh not found: " + name);
        }

        return it->second.mesh;
    }

    void MeshManager::unload(const std::string& name) {
        auto it = this->_meshMap.find(name);
        if (it == this->_meshMap.end()) {
            Logger::warning("Cannot unload missing mesh: " + name);
            return;
        }

        it->second.refCount--;
        if (it->second.refCount <= 0) {
            this->_meshMap.erase(it);
            Logger::info("Unloaded mesh: " + name);
        }
    }
}
