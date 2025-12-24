#include "ServiceLocator.hpp"

void ServiceLocator::shutdown() {
    for (auto it = this->_registrationOrder.rbegin(); it != this->_registrationOrder.rend(); ++it)
        this->_managers.erase(*it);

    this->_registrationOrder.clear();
}

void ServiceLocator::debugLogManagers() const {
    Krio::Logger::info("Registred Managers:");
    for (const auto& pair : this->_managers)
        Krio::Logger::info(" - " + std::string(pair.first.name()));
}