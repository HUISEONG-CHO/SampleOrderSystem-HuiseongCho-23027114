#include "JsonValue.h"

JsonValue::JsonValue() : type(Type::Null) {}
JsonValue::JsonValue(bool v) : type(Type::Bool), boolVal(v) {}
JsonValue::JsonValue(double v) : type(Type::Number), numVal(v) {}
JsonValue::JsonValue(const std::string& v) : type(Type::String), strVal(v) {}
JsonValue::JsonValue(std::string&& v) : type(Type::String), strVal(std::move(v)) {}

JsonValue JsonValue::makeArray()  { JsonValue v; v.type = Type::Array;  return v; }
JsonValue JsonValue::makeObject() { JsonValue v; v.type = Type::Object; return v; }

bool JsonValue::isNull()   const { return type == Type::Null;   }
bool JsonValue::isBool()   const { return type == Type::Bool;   }
bool JsonValue::isNumber() const { return type == Type::Number; }
bool JsonValue::isString() const { return type == Type::String; }
bool JsonValue::isArray()  const { return type == Type::Array;  }
bool JsonValue::isObject() const { return type == Type::Object; }

bool JsonValue::asBool() const {
    if (!isBool()) throw std::runtime_error("JsonValue is not bool");
    return boolVal;
}
double JsonValue::asDouble() const {
    if (!isNumber()) throw std::runtime_error("JsonValue is not number");
    return numVal;
}
const std::string& JsonValue::asString() const {
    if (!isString()) throw std::runtime_error("JsonValue is not string");
    return strVal;
}

void JsonValue::push(const JsonValue& v) {
    if (!isArray()) throw std::runtime_error("JsonValue is not array");
    arrVal.push_back(v);
}

size_t JsonValue::size() const {
    if (isArray())  return arrVal.size();
    if (isObject()) return objVal.size();
    return 0;
}

JsonValue& JsonValue::operator[](size_t index) {
    if (!isArray()) throw std::runtime_error("JsonValue is not array");
    return arrVal.at(index);
}
const JsonValue& JsonValue::operator[](size_t index) const {
    if (!isArray()) throw std::runtime_error("JsonValue is not array");
    return arrVal.at(index);
}

JsonValue& JsonValue::operator[](const std::string& key) {
    if (!isObject()) throw std::runtime_error("JsonValue is not object");
    return objVal[key];
}
const JsonValue& JsonValue::operator[](const std::string& key) const {
    if (!isObject()) throw std::runtime_error("JsonValue is not object");
    return objVal.at(key);
}

JsonValue::Type JsonValue::getType() const { return type; }

const std::vector<JsonValue>& JsonValue::asArray() const {
    if (!isArray()) throw std::runtime_error("JsonValue is not array");
    return arrVal;
}
const std::map<std::string, JsonValue>& JsonValue::asObject() const {
    if (!isObject()) throw std::runtime_error("JsonValue is not object");
    return objVal;
}
