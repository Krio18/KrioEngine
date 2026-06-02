#pragma once

#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include <mutex>
#include <stdexcept>

#include "../Logger/Logger.hpp"

namespace Voxel {
    class ServiceLocator {
        public:
            template<typename T, typename... Args>
            void registerManager(Args&&... args) {
                std::lock_guard<std::mutex> lock(this->_mutex);
                std::type_index typeId = std::type_index(typeid(T));
                Logger::info("Registering manager: " + std::string(typeId.name()));
                this->_managers[typeId] = std::make_unique<TypedManagerWrapper<T>>(std::forward<Args>(args)...);
                this->_registrationOrder.push_back(typeId);
            }

            template<typename T>
            T& getManager() {
                std::lock_guard<std::mutex> lock(this->_mutex);
                std::type_index typeId = std::type_index(typeid(T));
                auto it = this->_managers.find(typeId);
                if (it == this->_managers.end())
                    throw std::runtime_error("Manager not registered");
                return static_cast<TypedManagerWrapper<T>*>(it->second.get())->instance;
            }

            template<typename T>
            bool hasManager() const {
                std::lock_guard<std::mutex> lock(this->_mutex);
                std::type_index typeId = std::type_index(typeid(T));
                return this->_managers.find(typeId) != this->_managers.end();
            }

            void shutdown();
            void debugLogManagers() const;

        private:
            struct ManagerWrapper {
                virtual ~ManagerWrapper() = default;
            };

            template<typename T>
            struct TypedManagerWrapper : ManagerWrapper {
                T instance;

                template<typename... Args>
                TypedManagerWrapper(Args&&... args) : instance(std::forward<Args>(args)...) {}
            };

            std::unordered_map<std::type_index, std::unique_ptr<ManagerWrapper>> _managers;
            std::vector<std::type_index> _registrationOrder;
            mutable std::mutex _mutex;
    };
}
