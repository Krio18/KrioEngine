#pragma once

#include <iostream>
#include <string>

namespace Krio {
    enum class LogLevel {
        Info,
        Warning,
        Error
    };

    class Logger {
    public:
        static void log(LogLevel level, const std::string& message) {
            const char* prefixes[] = {"[INFO]", "[WARN]", "[ERROR]"};
            std::cout << prefixes[static_cast<int>(level)] << " " << message << std::endl;
        }

        static void info(const std::string& message) {
            log(LogLevel::Info, message);
        }

        static void warning(const std::string& message) {
            log(LogLevel::Warning, message);
        }

        static void error(const std::string& message) {
            log(LogLevel::Error, message);
        }
    };
}
