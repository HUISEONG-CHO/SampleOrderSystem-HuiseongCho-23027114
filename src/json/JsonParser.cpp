#include "JsonParser.h"
#include <cctype>
#include <cstdlib>

JsonParser::JsonParser(const std::string& text) : text(text), pos(0) {}

JsonValue JsonParser::parse() {
    skipWhitespace();
    if (pos >= text.size()) throw JsonParseError("Empty input");
    JsonValue v = parseValue();
    skipWhitespace();
    if (pos != text.size()) throw JsonParseError("Unexpected trailing characters");
    return v;
}

void JsonParser::skipWhitespace() {
    while (pos < text.size() && std::isspace((unsigned char)text[pos])) ++pos;
}

char JsonParser::current() const {
    if (pos >= text.size()) throw JsonParseError("Unexpected end of input");
    return text[pos];
}

char JsonParser::advance() {
    char c = current();
    ++pos;
    return c;
}

void JsonParser::expect(char c) {
    if (advance() != c)
        throw JsonParseError(std::string("Expected '") + c + "'");
}

JsonValue JsonParser::parseValue() {
    skipWhitespace();
    char c = current();
    if (c == '{') return parseObject();
    if (c == '[') return parseArray();
    if (c == '"') return JsonValue(parseString());
    if (c == 't' || c == 'f' || c == 'n') return parseLiteral();
    if (c == '-' || std::isdigit((unsigned char)c)) return parseNumber();
    throw JsonParseError(std::string("Unexpected character: ") + c);
}

JsonValue JsonParser::parseObject() {
    expect('{');
    JsonValue obj = JsonValue::makeObject();
    skipWhitespace();
    if (current() == '}') { advance(); return obj; }
    while (true) {
        skipWhitespace();
        if (current() != '"') throw JsonParseError("Expected string key");
        std::string key = parseString();
        skipWhitespace();
        expect(':');
        skipWhitespace();
        obj[key] = parseValue();
        skipWhitespace();
        char sep = current();
        if (sep == '}') { advance(); break; }
        if (sep != ',') throw JsonParseError("Expected ',' or '}'");
        advance();
    }
    return obj;
}

JsonValue JsonParser::parseArray() {
    expect('[');
    JsonValue arr = JsonValue::makeArray();
    skipWhitespace();
    if (current() == ']') { advance(); return arr; }
    while (true) {
        skipWhitespace();
        arr.push(parseValue());
        skipWhitespace();
        char sep = current();
        if (sep == ']') { advance(); break; }
        if (sep != ',') throw JsonParseError("Expected ',' or ']'");
        advance();
    }
    return arr;
}

std::string JsonParser::parseString() {
    expect('"');
    std::string result;
    while (true) {
        if (pos >= text.size()) throw JsonParseError("Unterminated string");
        char c = advance();
        if (c == '"') break;
        if (c == '\\') {
            char esc = advance();
            switch (esc) {
                case '"':  result += '"';  break;
                case '\\': result += '\\'; break;
                case '/':  result += '/';  break;
                case 'n':  result += '\n'; break;
                case 'r':  result += '\r'; break;
                case 't':  result += '\t'; break;
                case 'b':  result += '\b'; break;
                case 'f':  result += '\f'; break;
                default: throw JsonParseError(std::string("Invalid escape: \\") + esc);
            }
        } else {
            result += c;
        }
    }
    return result;
}

JsonValue JsonParser::parseNumber() {
    size_t start = pos;
    if (pos < text.size() && text[pos] == '-') ++pos;
    while (pos < text.size() && std::isdigit((unsigned char)text[pos])) ++pos;
    if (pos < text.size() && text[pos] == '.') {
        ++pos;
        while (pos < text.size() && std::isdigit((unsigned char)text[pos])) ++pos;
    }
    if (pos < text.size() && (text[pos] == 'e' || text[pos] == 'E')) {
        ++pos;
        if (pos < text.size() && (text[pos] == '+' || text[pos] == '-')) ++pos;
        while (pos < text.size() && std::isdigit((unsigned char)text[pos])) ++pos;
    }
    std::string numStr = text.substr(start, pos - start);
    return JsonValue(std::stod(numStr));
}

JsonValue JsonParser::parseLiteral() {
    if (text.compare(pos, 4, "true") == 0)  { pos += 4; return JsonValue(true);  }
    if (text.compare(pos, 5, "false") == 0) { pos += 5; return JsonValue(false); }
    if (text.compare(pos, 4, "null") == 0)  { pos += 4; return JsonValue();      }
    throw JsonParseError("Unknown literal at position " + std::to_string(pos));
}
