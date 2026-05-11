#include "Logger.hpp"

namespace Krio {
    void Logger::log(LogLevel level, const std::string& message) {
        const char* prefixes[] = {"[INFO]", "[WARN]", "[ERROR]"};
        std::cout << prefixes[static_cast<int>(level)] << " " << message << std::endl;
    }

    void Logger::info(const std::string& message) {
        log(LogLevel::Info, message);
    }

    void Logger::warning(const std::string& message) {
        log(LogLevel::Warning, message);
    }

    void Logger::error(const std::string& message) {
        log(LogLevel::Error, message);
    }
}
