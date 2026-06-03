#pragma once

#include <entt/entt.hpp>

namespace Voxel {
    class Registry;

    class Entity {
        public:
            Entity(entt::entity id, Registry* registry);
            ~Entity() = default;

            void destroy();
            bool isValid() const;
            entt::entity getID() const;

            bool operator==(const Entity& other) const;
            bool operator!=(const Entity& other) const;

            template<typename T, typename... Args>
            void addComponent(Args... args);

            template<typename T>
            T* getComponent();

            template<typename T>
            bool hasComponent();

            template<typename T>
            void removeComponent();

        private:
            Registry* _registry;
            entt::entity _ID;

    };
}
