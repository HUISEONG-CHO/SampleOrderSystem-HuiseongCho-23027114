#include "SampleController.h"
#include <stdexcept>

SampleController::SampleController(SampleRepository& repo) : repo(repo) {}

std::string SampleController::generateId() const {
    return "S" + std::to_string(repo.findAll().size() + 1);
}

void SampleController::registerSample(const std::string& name, int avgProductionTime, double yieldRate) {
    repo.save(Sample(generateId(), name, avgProductionTime, yieldRate, 0));
}

Sample SampleController::getSample(const std::string& id) const {
    return repo.findById(id);
}

std::vector<Sample> SampleController::getAllSamples() const {
    return repo.findAll();
}

std::vector<Sample> SampleController::searchByKeyword(const std::string& keyword) const {
    std::vector<Sample> result;
    for (const auto& s : repo.findAll()) {
        if (s.getId().find(keyword)   != std::string::npos ||
            s.getName().find(keyword) != std::string::npos ||
            std::to_string(s.getAvgProductionTime()).find(keyword) != std::string::npos ||
            std::to_string(s.getStock()).find(keyword) != std::string::npos ||
            std::to_string(s.getYieldRate()).find(keyword) != std::string::npos)
            result.push_back(s);
    }
    return result;
}
