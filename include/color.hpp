#ifndef COLOR_HPP
#define COLOR_HPP

#ifdef _WIN32
    #define NOMINMAX
    #include <Windows.h>
#endif

class Color {
public:
    enum Code {
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        DEFAULT = 39,
    };

    static std::string colorize(const std::string& text, Code code, bool bold = false) {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleMode(hOut, ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
        std::string ansiCode = "\033[";
        if (bold)
            ansiCode += "1;";
        ansiCode += std::to_string(code) + "m";
        return ansiCode + text + "\033[0m";
    }
};

#endif  // COLOR_HPP