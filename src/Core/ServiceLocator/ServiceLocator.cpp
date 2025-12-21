#include "ServiceLocator.hpp"

void ServiceLocator::shutdown() {
    this->_managers.clear();
}

void ServiceLocator::debugLogManagers() const {
    Krio::Logger::info("Registred Managers:");
    for (const auto& pair : this->_managers)
        Krio::Logger::info(" - " + std::string(pair.first.name()));
}