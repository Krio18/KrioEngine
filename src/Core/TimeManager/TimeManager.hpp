#pragma once

#include "../Logger.hpp"

#include <chrono>

namespace Krio {
    class TimeManager {
        public:
            TimeManager();
            ~TimeManager() = default;

            void update();
            void setTimeScale(double scale);

            double getFPS() const;
            double getDeltaTime() const;
            double getUnscaledDeltaTime() const;
            double getElapsedTime() const;
            double getTimeScale() const;

            void debugLogTimeInfo() const;

        private:
            static constexpr double SMOOTHING_FACTOR = 0.1;
            static constexpr double MAX_DELTA_TIME = 0.1;

            int _FPS;
            double _deltaTime;
            double _unscaledDeltaTime;
            double _elapsedTime;
            double _timeScale;
            double _smoothedDeltaTime;
            std::chrono::steady_clock::time_point _lastFrameTime;
    };
}
