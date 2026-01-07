#pragma once

#include "../Logger.hpp"

class PhysicsManager {
    public:
        PhysicsManager();
        ~PhysicsManager() = default;

        void fixedUpdate(double fixedDeltaTime);
        void setFixedTimestep(double timestep);
        double getFixedTimestep() const;

    private:
        static constexpr double DEFAULT_FIXED_TIMESTEP = 1.0 / 60.0;
        double _fixedTimestep;
};