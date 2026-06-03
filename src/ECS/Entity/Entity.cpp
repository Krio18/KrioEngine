#include "Entity.hpp"

namespace Voxel {
    Entity::Entity(entt::entity id, Registry* registry) : _ID(id), _registry(registry) {}
    Entity::~Entity() {}
}
