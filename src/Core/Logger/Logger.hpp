#pragma once

#include <iostream>
#include <string>

namespace Voxel {
    enum class LogLevel {
        Info,
        Warning,
        Error
    };

    class Logger {
        public:
            static void log(LogLevel level, const std::string& message);
            static void info(const std::string& message);
            static void warning(const std::string& message);
            static void error(const std::string& message);
    };
}
