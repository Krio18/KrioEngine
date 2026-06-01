#include "Logger.hpp"

namespace Voxel {
    void Logger::log(LogLevel level, const std::string& message) {
        const char* prefixes[] = {"[INFO]", "[WARN]", "[ERROR]"};
        const char* colors[] = {"\033[1;34m", "\033[1;33m", "\033[1;31m"};
        const char* reset = "\033[0m";

        std::cout << colors[static_cast<int>(level)]
                  << prefixes[static_cast<int>(level)]
                  << reset << " " << message << std::endl;
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
