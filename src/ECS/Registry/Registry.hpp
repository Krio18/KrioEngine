#pragma once

#include "../Entity/Entity.hpp"
#include "../Components/Components.hpp"

namespace Voxel {
    class Registry {
        public:
            Registry();
            ~Registry() = default;

            Entity createEntity();
            void destroyEntity(Entity entity);
            bool isValid(entt::entity id) const;

            template<typename T, typename... Args>
            void addComponent(Entity entity, Args... args) {
                this->_registry.emplace<T>(entity.getID(), args...);
            }

            template<typename T>
            T* getComponent(Entity entity) {
                return this->_registry.try_get<T>(entity.getID());
            }

            template<typename T>
            bool hasComponent(Entity entity) {
                return this->_registry.any_of<T>(entity.getID());
            }

            template<typename T>
            void removeComponent(Entity entity) {
                this->_registry.remove<T>(entity.getID());
            }

            template<typename... Components>
            auto view() {
                return _registry.view<Components...>();
            }

            template<typename... Components, typename Callback>
            void each(Callback callback) {
                return;
            }

        private:
            entt::registry _registry;
    };

    template<typename T, typename... Args>
    inline void Entity::addComponent(Args... args) {
        this->_registry->template addComponent<T>(*this, args...);
    }

    template<typename T>
    inline T* Entity::getComponent() {
        return this->_registry->template getComponent<T>(*this);
    }

    template<typename T>
    inline bool Entity::hasComponent() {
        return this->_registry->template hasComponent<T>(*this);
    }

    template<typename T>
    inline void Entity::removeComponent() {
        this->_registry->template removeComponent<T>(*this);
    }
}
