#pragma once
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

class JsonParseError : public std::runtime_error {
public:
    explicit JsonParseError(const std::string& msg) : std::runtime_error(msg) {}
};

class JsonValue {
public:
    enum class Type { Null, Bool, Number, String, Array, Object };

    JsonValue();
    explicit JsonValue(bool v);
    explicit JsonValue(double v);
    explicit JsonValue(const std::string& v);
    explicit JsonValue(std::string&& v);

    static JsonValue makeArray();
    static JsonValue makeObject();

    bool isNull()   const;
    bool isBool()   const;
    bool isNumber() const;
    bool isString() const;
    bool isArray()  const;
    bool isObject() const;

    bool               asBool()   const;
    double             asDouble() const;
    const std::string& asString() const;

    void   push(const JsonValue& v);
    size_t size() const;

    JsonValue&       operator[](size_t index);
    const JsonValue& operator[](size_t index) const;
    JsonValue&       operator[](const std::string& key);
    const JsonValue& operator[](const std::string& key) const;

    Type getType() const;

    const std::vector<JsonValue>&            asArray()  const;
    const std::map<std::string, JsonValue>&  asObject() const;

private:
    Type type;
    bool   boolVal  = false;
    double numVal   = 0.0;
    std::string strVal;
    std::vector<JsonValue> arrVal;
    std::map<std::string, JsonValue> objVal;
};
