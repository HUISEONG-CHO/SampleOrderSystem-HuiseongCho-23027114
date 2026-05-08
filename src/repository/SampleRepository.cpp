#include "SampleRepository.h"
#include "json/json.h"
#include <filesystem>
#include <stdexcept>
#include <algorithm>

namespace fs = std::filesystem;

SampleRepository::SampleRepository(const std::string& filePath) : filePath(filePath) {
    if (fs::exists(filePath)) loadFromFile();
}

void SampleRepository::loadFromFile() {
    JsonValue data = Json::parseFile(filePath);
    for (size_t i = 0; i < data.size(); ++i) {
        const JsonValue& o = data[i];
        samples.emplace_back(
            o["id"].asString(),
            o["name"].asString(),
            static_cast<int>(o["avgProductionTime"].asDouble()),
            o["yieldRate"].asDouble(),
            static_cast<int>(o["stock"].asDouble())
        );
    }
}

void SampleRepository::saveToFile() const {
    JsonValue arr = JsonValue::makeArray();
    for (const auto& s : samples) {
        JsonValue obj = JsonValue::makeObject();
        obj["id"]                = JsonValue(s.getId());
        obj["name"]              = JsonValue(s.getName());
        obj["avgProductionTime"] = JsonValue(static_cast<double>(s.getAvgProductionTime()));
        obj["yieldRate"]         = JsonValue(s.getYieldRate());
        obj["stock"]             = JsonValue(static_cast<double>(s.getStock()));
        arr.push(obj);
    }
    Json::saveFile(arr, filePath, true);
}

void SampleRepository::save(const Sample& item) {
    samples.push_back(item);
    saveToFile();
}

Sample SampleRepository::findById(const std::string& id) const {
    for (const auto& s : samples)
        if (s.getId() == id) return s;
    throw std::runtime_error("Sample not found: " + id);
}

std::vector<Sample> SampleRepository::findAll() const { return samples; }

void SampleRepository::update(const std::string& id, const Sample& item) {
    for (auto& s : samples)
        if (s.getId() == id) { s = item; saveToFile(); return; }
    throw std::runtime_error("Sample not found: " + id);
}

void SampleRepository::remove(const std::string& id) {
    samples.erase(std::remove_if(samples.begin(), samples.end(),
        [&id](const Sample& s) { return s.getId() == id; }), samples.end());
    saveToFile();
}
