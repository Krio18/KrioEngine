#include "Entity.hpp"
#include "../Registry/Registry.hpp"

namespace Voxel {
    Entity::Entity(entt::entity id, Registry* registry) : _registry(registry), _ID(id) {}

    void Entity::destroy() {
        if (this->_registry != nullptr) {
            this->_registry->destroyEntity(*this);
        }
    }

    bool Entity::isValid() const {
        if (this->_registry != nullptr) {
            return this->_registry->isValid(this->_ID);
        }

        return false;
    }

    entt::entity Entity::getID() const {
        return this->_ID;
    }

    bool Entity::operator==(const Entity& other) const {
        return (this->_ID == other._ID) && (this->_registry == other._registry);
    }

    bool Entity::operator!=(const Entity& other) const {
        return !(*this == other);
    }

}
