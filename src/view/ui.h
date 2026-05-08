#pragma once
#include <string>
#include <sstream>
#include <iomanip>

namespace UI {
    // ANSI colors
    constexpr const char* RESET    = "\033[0m";
    constexpr const char* BOLD     = "\033[1m";
    constexpr const char* DIM      = "\033[2m";
    constexpr const char* RED      = "\033[91m";
    constexpr const char* GREEN    = "\033[92m";
    constexpr const char* YELLOW   = "\033[93m";
    constexpr const char* BLUE     = "\033[94m";
    constexpr const char* CYAN     = "\033[96m";
    constexpr const char* B_GREEN  = "\033[1;92m";
    constexpr const char* B_RED    = "\033[1;91m";
    constexpr const char* B_YELLOW = "\033[1;93m";
    constexpr const char* B_CYAN   = "\033[1;96m";
    constexpr const char* B_WHITE  = "\033[1;97m";

    // UTF-8 aware display width: Korean/CJK = 2, ASCII = 1
    inline size_t displayWidth(const std::string& s) {
        size_t w = 0;
        for (size_t i = 0; i < s.size(); ) {
            unsigned char c = s[i];
            if      (c < 0x80) { w += 1; i += 1; }
            else if (c < 0xE0) { w += 1; i += 2; }
            else if (c < 0xF0) { w += 2; i += 3; }
            else               { w += 2; i += 4; }
        }
        return w;
    }

    inline std::string padRight(const std::string& s, size_t w) {
        size_t dw = displayWidth(s);
        return dw >= w ? s : s + std::string(w - dw, ' ');
    }

    inline std::string padLeft(const std::string& s, size_t w) {
        size_t dw = displayWidth(s);
        return dw >= w ? s : std::string(w - dw, ' ') + s;
    }

    inline std::string fmtDouble(double v, int prec = 1) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(prec) << v;
        return oss.str();
    }

    // repeat a UTF-8 string n times
    inline std::string rep(const std::string& s, int n) {
        std::string r;
        r.reserve(s.size() * n);
        for (int i = 0; i < n; ++i) r += s;
        return r;
    }
}
