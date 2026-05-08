#pragma once
#include "repository/SampleRepository.h"
#include <string>
#include <vector>

class SampleController {
public:
    explicit SampleController(const std::string& filePath);

    void               registerSample(const std::string& name, int avgProductionTime, double yieldRate);
    Sample             getSample(const std::string& id) const;
    std::vector<Sample> getAllSamples() const;
    std::vector<Sample> searchByName(const std::string& keyword) const;

private:
    SampleRepository repo;
    std::string      generateId() const;
};
