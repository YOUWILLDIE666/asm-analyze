#ifndef COLOR_HPP
#define COLOR_HPP

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
        std::string ansiCode = "\033[";
        if (bold)
            ansiCode += "1;";
        ansiCode += std::to_string(code) + "m";
        return ansiCode + text + "\033[0m";
    }
};

#endif  // COLOR_HPP