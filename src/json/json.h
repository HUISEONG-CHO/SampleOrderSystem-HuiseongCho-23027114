#pragma once
#include "JsonValue.h"
#include "JsonParser.h"
#include "JsonSerializer.h"
#include <fstream>
#include <sstream>

namespace Json {

inline JsonValue parse(const std::string& text) {
    return JsonParser(text).parse();
}

inline JsonValue parseFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) throw JsonParseError("Cannot open file: " + path);
    std::string text((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
    return parse(text);
}

inline std::string stringify(const JsonValue& value, bool pretty = false, int indent = 4) {
    return JsonSerializer(pretty, indent).serialize(value);
}

inline void saveFile(const JsonValue& value, const std::string& path,
                     bool pretty = true, int indent = 4) {
    std::ofstream file(path);
    if (!file.is_open()) throw std::runtime_error("Cannot write file: " + path);
    file << stringify(value, pretty, indent);
}

} // namespace Json
