#ifndef DBG_HPP
#define DBG_HPP

#include "color.hpp"
#include <iostream>
#include <string>

namespace dbg {
    class Debugger {
    public:
        enum Level {
            DEBUG,
            INFO,
            WARN,
            ERR,
            FATAL
        };

        static void log(const std::string& message, Level level, const std::string& file = "", int line = 0) {
            std::string l;
            Color::Code cc;

            switch (level) {
            case DEBUG:
                l = "DEBUG";
                cc = Color::BLUE;
                break;
            case INFO:
                l = "INFO";
                cc = Color::GREEN;
                break;
            case WARN:
                l = "WARN";
                cc = Color::YELLOW;
                break;
            case ERR:
                l = "ERROR";
                cc = Color::RED;
                break;
            case FATAL:
                l = "FATAL";
                cc = Color::RED;
                break;
            }

            std::string colored = Color::colorize(l, cc);

            std::cout << "[" << colored << "] ";
            if (!file.empty()) {
                std::cout << file << ":" << line << " ";
            }
            std::cout << message << std::endl;
        }

        static void debug(const std::string& message, const std::string& file = "", int line = 0) {
            log(message, DEBUG, file, line);
        }

        static void info(const std::string& message, const std::string& file = "", int line = 0) {
            log(message, INFO, file, line);
        }

        static void warn(const std::string& message, const std::string& file = "", int line = 0) {
            log(message, WARN, file, line);
        }

        static void error(const std::string& message, const std::string& file = "", int line = 0) {
            log(message, ERR, file, line);
        }

        static void fatal(const std::string& message, const std::string& file = "", int line = 0) {
            log(message, FATAL, file, line);
        }
    };

    namespace Macros {
        #define DEBUG(message) dbg::Debugger::debug(message, __FILE__, __LINE__)
        #define INFO(message) dbg::Debugger::info(message, __FILE__, __LINE__)
        #define WARN(message) dbg::Debugger::warn(message, __FILE__, __LINE__)
        #define ERROR(message) dbg::Debugger::error(message, __FILE__, __LINE__)
        #define FATAL(message) dbg::Debugger::fatal(message, __FILE__, __LINE__)
    }
}

#endif  // DBG_HPP