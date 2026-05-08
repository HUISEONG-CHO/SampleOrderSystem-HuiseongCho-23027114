#pragma once
#include "JsonValue.h"
#include <string>

class JsonSerializer {
public:
    explicit JsonSerializer(bool pretty = false, int indent = 4);
    std::string serialize(const JsonValue& value) const;

private:
    bool pretty;
    int  indent;

    std::string serializeValue(const JsonValue& v, int depth) const;
    std::string serializeString(const std::string& s) const;
    std::string serializeNumber(double v) const;
    std::string indentStr(int depth) const;
};
