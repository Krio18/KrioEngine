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
                return;
            }

            template<typename T>
            T* getComponent() {
                return nullptr;
            }

            template<typename T>
            bool hasComponent() {
                return false;
            }

            template<typename T>
            void removeComponent() {
                return;
            }

            void destroy();
            bool isValid() const;
            entt::entity getID() const;

        private:
            Registry* _registry;
            entt::entity _ID;

    };
}
