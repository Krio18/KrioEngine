#include "Registry.hpp"

namespace Voxel {
    Registry::Registry() {}

    Entity Registry::createEntity() {
        return Entity(this->_registry.create(), this);
    }

    void Registry::destroyEntity(Entity entity) {
        this->_registry.destroy(entity.getID());
    }

    bool Registry::isValid(entt::entity id) const {
        return this->_registry.valid(id);
    }
}
