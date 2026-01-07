#include "ServiceLocator.hpp"

void ServiceLocator::shutdown() {
    Krio::Logger::info("Shutting down managers in reverse order...");
    for (auto it = this->_registrationOrder.rbegin(); it != this->_registrationOrder.rend(); ++it) {
        Krio::Logger::info("Destroying manager: " + std::string(it->name()));
        this->_managers.erase(*it);
    }

    this->_registrationOrder.clear();
    Krio::Logger::info("All managers destroyed");
}

void ServiceLocator::debugLogManagers() const {
    Krio::Logger::info("Registred Managers:");
    for (const auto& pair : this->_managers)
        Krio::Logger::info(" - " + std::string(pair.first.name()));
}