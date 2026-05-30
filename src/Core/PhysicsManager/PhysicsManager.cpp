#include "PhysicsManager.hpp"

namespace Voxel {
    PhysicsManager::PhysicsManager()
        : _fixedTimestep(DEFAULT_FIXED_TIMESTEP)
    {
        Logger::info("PhysicsManager created");
    }

    void PhysicsManager::fixedUpdate(double fixedDeltaTime) {}

    void PhysicsManager::setFixedTimestep(double timestep) {
        this->_fixedTimestep = timestep;
    }

    double PhysicsManager::getFixedTimestep() const {
        return this->_fixedTimestep;
    }
}