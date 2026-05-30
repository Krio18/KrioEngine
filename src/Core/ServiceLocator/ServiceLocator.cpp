#include "ServiceLocator.hpp"

namespace Voxel {
    void ServiceLocator::shutdown() {
        Logger::info("Shutting down managers in reverse order...");
        for (auto it = this->_registrationOrder.rbegin(); it != this->_registrationOrder.rend(); ++it) {
            Logger::info("Destroying manager: " + std::string(it->name()));
            this->_managers.erase(*it);
        }

        this->_registrationOrder.clear();
        Logger::info("All managers destroyed");
    }

    void ServiceLocator::debugLogManagers() const {
        Logger::info("Registred Managers:");
        for (const auto& pair : this->_managers)
            Logger::info(" - " + std::string(pair.first.name()));
    }
}