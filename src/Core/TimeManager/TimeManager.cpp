#include "TimeManager.hpp"

TimeManager::TimeManager() :
    _FPS(0),
    _deltaTime(0.0),
    _unscaledDeltaTime(0.0),
    _elapsedTime(0.0),
    _timeScale(1.0),
    _smoothedDeltaTime(0.0),
    _lastFrameTime(std::chrono::steady_clock::now())
{}

void TimeManager::update() {
    auto now = std::chrono::steady_clock::now();
    double rawDeltaTime = std::chrono::duration<double>(now - this->_lastFrameTime).count();
    this->_lastFrameTime = now;

    rawDeltaTime = std::min(rawDeltaTime, MAX_DELTA_TIME);

    this->_unscaledDeltaTime = rawDeltaTime;

    if (this->_smoothedDeltaTime == 0.0)
        this->_smoothedDeltaTime = rawDeltaTime;
    else
        this->_smoothedDeltaTime = (this->_smoothedDeltaTime * (1.0 - SMOOTHING_FACTOR)) + (rawDeltaTime * SMOOTHING_FACTOR);

    this->_deltaTime = this->_smoothedDeltaTime * this->_timeScale;
    this->_elapsedTime += this->_deltaTime;

    if (this->_unscaledDeltaTime > 0.0)
        this->_FPS = static_cast<int>(1.0 / this->_unscaledDeltaTime);
    else
        this->_FPS = 0;
}

void TimeManager::setTimeScale(double scale) {
    this->_timeScale = scale;
}

double TimeManager::getFPS() const {
    return this->_FPS;
}

double TimeManager::getDeltaTime() const {
    return this->_deltaTime;
}

double TimeManager::getUnscaledDeltaTime() const {
    return this->_unscaledDeltaTime;
}

double TimeManager::getElapsedTime() const {
    return this->_elapsedTime;
}

double TimeManager::getTimeScale() const {
    return this->_timeScale;
}

void TimeManager::debugLogTimeInfo() const {
    Krio::Logger::info("Time Info :");
    Krio::Logger::info(" - FPS : " + std::to_string(this->getFPS()));
    Krio::Logger::info(" - Delta Time : " + std::to_string(this->getDeltaTime()));
    Krio::Logger::info(" - Unscaled Delta Time : " + std::to_string(this->getUnscaledDeltaTime()));
    Krio::Logger::info(" - Elapsed Time : " + std::to_string(this->getElapsedTime()));
    Krio::Logger::info(" - Time Scale : " + std::to_string(this->getTimeScale()));
}