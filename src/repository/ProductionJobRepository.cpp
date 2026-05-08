#include "ProductionJobRepository.h"
#include "json/json.h"
#include <filesystem>
#include <stdexcept>
#include <algorithm>

namespace fs = std::filesystem;

ProductionJobRepository::ProductionJobRepository(const std::string& filePath)
    : filePath(filePath) {
    if (fs::exists(filePath)) loadFromFile();
}

void ProductionJobRepository::loadFromFile() {
    JsonValue data = Json::parseFile(filePath);
    for (size_t i = 0; i < data.size(); ++i) {
        const JsonValue& o = data[i];
        jobs.emplace_back(
            o["orderId"].asString(),
            o["sampleId"].asString(),
            static_cast<int>(o["productionQty"].asDouble()),
            static_cast<int>(o["totalTime"].asDouble()),
            o["enqueuedAt"].asString()
        );
    }
}

void ProductionJobRepository::saveToFile() const {
    JsonValue arr = JsonValue::makeArray();
    for (const auto& job : jobs) {
        JsonValue obj = JsonValue::makeObject();
        obj["orderId"]       = JsonValue(job.getOrderId());
        obj["sampleId"]      = JsonValue(job.getSampleId());
        obj["productionQty"] = JsonValue(static_cast<double>(job.getProductionQty()));
        obj["totalTime"]     = JsonValue(static_cast<double>(job.getTotalTime()));
        obj["enqueuedAt"]    = JsonValue(job.getEnqueuedAt());
        arr.push(obj);
    }
    Json::saveFile(arr, filePath, true);
}

void ProductionJobRepository::save(const ProductionJob& item) {
    jobs.push_back(item);
    saveToFile();
}

ProductionJob ProductionJobRepository::findById(const std::string& orderId) const {
    for (const auto& j : jobs)
        if (j.getOrderId() == orderId) return j;
    throw std::runtime_error("ProductionJob not found: " + orderId);
}

std::vector<ProductionJob> ProductionJobRepository::findAll() const { return jobs; }

void ProductionJobRepository::update(const std::string& orderId, const ProductionJob& item) {
    for (auto& j : jobs)
        if (j.getOrderId() == orderId) { j = item; saveToFile(); return; }
    throw std::runtime_error("ProductionJob not found: " + orderId);
}

void ProductionJobRepository::remove(const std::string& orderId) {
    jobs.erase(std::remove_if(jobs.begin(), jobs.end(),
        [&orderId](const ProductionJob& j) { return j.getOrderId() == orderId; }), jobs.end());
    saveToFile();
}
