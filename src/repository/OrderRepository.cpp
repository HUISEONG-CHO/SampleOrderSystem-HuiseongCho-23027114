#include "OrderRepository.h"
#include "json/json.h"
#include <filesystem>
#include <stdexcept>
#include <algorithm>

namespace fs = std::filesystem;

OrderRepository::OrderRepository(const std::string& filePath) : filePath(filePath) {
    if (fs::exists(filePath)) loadFromFile();
}

std::string OrderRepository::statusToString(OrderStatus s) {
    switch (s) {
        case OrderStatus::RESERVED:  return "RESERVED";
        case OrderStatus::REJECTED:  return "REJECTED";
        case OrderStatus::PRODUCING: return "PRODUCING";
        case OrderStatus::CONFIRMED: return "CONFIRMED";
        case OrderStatus::RELEASE:   return "RELEASE";
    }
    return "RESERVED";
}

OrderStatus OrderRepository::stringToStatus(const std::string& s) {
    if (s == "REJECTED")  return OrderStatus::REJECTED;
    if (s == "PRODUCING") return OrderStatus::PRODUCING;
    if (s == "CONFIRMED") return OrderStatus::CONFIRMED;
    if (s == "RELEASE")   return OrderStatus::RELEASE;
    return OrderStatus::RESERVED;
}

void OrderRepository::loadFromFile() {
    JsonValue data = Json::parseFile(filePath);
    for (size_t i = 0; i < data.size(); ++i) {
        const JsonValue& o = data[i];
        Order order(
            o["id"].asString(),
            o["sampleId"].asString(),
            o["customer"].asString(),
            static_cast<int>(o["quantity"].asDouble()),
            o["createdAt"].asString()
        );
        order.setStatus(stringToStatus(o["status"].asString()));
        orders.push_back(order);
    }
}

void OrderRepository::saveToFile() const {
    JsonValue arr = JsonValue::makeArray();
    for (const auto& order : orders) {
        JsonValue obj = JsonValue::makeObject();
        obj["id"]        = JsonValue(order.getId());
        obj["sampleId"]  = JsonValue(order.getSampleId());
        obj["customer"]  = JsonValue(order.getCustomer());
        obj["quantity"]  = JsonValue(static_cast<double>(order.getQuantity()));
        obj["status"]    = JsonValue(statusToString(order.getStatus()));
        obj["createdAt"] = JsonValue(order.getCreatedAt());
        arr.push(obj);
    }
    Json::saveFile(arr, filePath, true);
}

void OrderRepository::save(const Order& item) {
    orders.push_back(item);
    saveToFile();
}

Order OrderRepository::findById(const std::string& id) const {
    for (const auto& o : orders)
        if (o.getId() == id) return o;
    throw std::runtime_error("Order not found: " + id);
}

std::vector<Order> OrderRepository::findAll() const { return orders; }

void OrderRepository::update(const std::string& id, const Order& item) {
    for (auto& o : orders)
        if (o.getId() == id) { o = item; saveToFile(); return; }
    throw std::runtime_error("Order not found: " + id);
}

void OrderRepository::remove(const std::string& id) {
    orders.erase(std::remove_if(orders.begin(), orders.end(),
        [&id](const Order& o) { return o.getId() == id; }), orders.end());
    saveToFile();
}

std::vector<Order> OrderRepository::findByStatus(OrderStatus status) const {
    std::vector<Order> result;
    for (const auto& o : orders)
        if (o.getStatus() == status) result.push_back(o);
    return result;
}
