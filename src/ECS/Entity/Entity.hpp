#pragma once

#include <entt/entt.hpp>

namespace Voxel {
    class Registry;

    class Entity {
        public:
            Entity(entt::entity id, Registry* registry);
            ~Entity() = default;

            template<typename T, typename... Args>
            void addComponent(Args... args) {
                this->_registry->addComponent<T>(*this, args...);
            }

            template<typename T>
            T* getComponent() {
                return this->_registry->getComponent<T>(*this);
            }

            template<typename T>
            bool hasComponent() {
                return this->_registry->hasComponent<T>(*this);
            }

            template<typename T>
            void removeComponent() {
                this->_registry->removeComponent<T>(*this);
            }

            void destroy();
            bool isValid() const;
            entt::entity getID() const;

            bool operator==(const Entity& other) const;
            bool operator!=(const Entity& other) const;

        private:
            Registry* _registry;
            entt::entity _ID;

    };
}
