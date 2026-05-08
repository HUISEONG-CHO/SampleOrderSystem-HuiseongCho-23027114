#pragma once
#include "JsonValue.h"
#include <string>

class JsonParser {
public:
    explicit JsonParser(const std::string& text);
    JsonValue parse();

private:
    const std::string& text;
    size_t pos;

    void skipWhitespace();
    char  current() const;
    char  advance();
    void  expect(char c);

    JsonValue   parseValue();
    JsonValue   parseObject();
    JsonValue   parseArray();
    std::string parseString();
    JsonValue   parseNumber();
    JsonValue   parseLiteral();
};
