#ifndef DBG_HPP
#define DBG_HPP

#include "color.hpp"
#include <iostream>
#include <string>

namespace dbg {
    class Debugger {
    public:
        enum Level {
            INFO,
            WARN,
            ERR,
            FATAL
        };

        static void log(const std::string& message, Level level) {
            std::string l;
            Color::Code cc;

            switch (level) {
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
            std::cout << message << std::endl;
        }

        static void info(const std::string& message) {
            log(message, INFO);
        }

        static void warn(const std::string& message) {
            log(message, WARN);
        }

        static void error(const std::string& message) {
            log(message, ERR);
        }

        static void fatal(const std::string& message) {
            log(message, FATAL);
        }
    };

    namespace Macros {
        #define _INFO(message) dbg::Debugger::info(message)
        #define _WARN(message) dbg::Debugger::warn(message)
        #define _ERROR(message) dbg::Debugger::error(message)
        #define _FATAL(message) dbg::Debugger::fatal(message)
    }
}

#endif  // DBG_HPP
