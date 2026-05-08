#include "JsonSerializer.h"
#include <sstream>
#include <cstdio>

JsonSerializer::JsonSerializer(bool pretty, int indent)
    : pretty(pretty), indent(indent) {}

std::string JsonSerializer::serialize(const JsonValue& value) const {
    return serializeValue(value, 0);
}

std::string JsonSerializer::indentStr(int depth) const {
    if (!pretty) return "";
    return std::string(depth * indent, ' ');
}

std::string JsonSerializer::serializeString(const std::string& s) const {
    std::string result = "\"";
    for (char c : s) {
        switch (c) {
            case '"':  result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n";  break;
            case '\r': result += "\\r";  break;
            case '\t': result += "\\t";  break;
            default:   result += c;      break;
        }
    }
    result += '"';
    return result;
}

std::string JsonSerializer::serializeNumber(double v) const {
    char buf[64];
    snprintf(buf, sizeof(buf), "%g", v);
    return buf;
}

std::string JsonSerializer::serializeValue(const JsonValue& v, int depth) const {
    switch (v.getType()) {
        case JsonValue::Type::Null:   return "null";
        case JsonValue::Type::Bool:   return v.asBool() ? "true" : "false";
        case JsonValue::Type::Number: return serializeNumber(v.asDouble());
        case JsonValue::Type::String: return serializeString(v.asString());

        case JsonValue::Type::Array: {
            const auto& arr = v.asArray();
            if (arr.empty()) return "[]";
            std::string sep  = pretty ? "\n" : "";
            std::string csep = pretty ? ",\n" : ",";
            std::string inner;
            for (size_t i = 0; i < arr.size(); ++i) {
                if (i > 0) inner += csep;
                inner += indentStr(depth + 1) + serializeValue(arr[i], depth + 1);
            }
            return "[" + sep + inner + sep + indentStr(depth) + "]";
        }

        case JsonValue::Type::Object: {
            const auto& obj = v.asObject();
            if (obj.empty()) return "{}";
            std::string sep  = pretty ? "\n" : "";
            std::string csep = pretty ? ",\n" : ",";
            std::string kvsep = pretty ? ": " : ":";
            std::string inner;
            bool first = true;
            for (const auto& [key, val] : obj) {
                if (!first) inner += csep;
                inner += indentStr(depth + 1) + serializeString(key) + kvsep
                       + serializeValue(val, depth + 1);
                first = false;
            }
            return "{" + sep + inner + sep + indentStr(depth) + "}";
        }
    }
    return "null";
}
