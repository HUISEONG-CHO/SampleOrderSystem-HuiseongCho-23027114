#pragma once
#include "repository/SampleRepository.h"
#include <string>
#include <vector>

class SampleController {
public:
    explicit SampleController(SampleRepository& repo);

    void               registerSample(const std::string& name, int avgProductionTime, double yieldRate);
    Sample             getSample(const std::string& id) const;
    std::vector<Sample> getAllSamples() const;
    std::vector<Sample> searchByKeyword(const std::string& keyword) const;
    void               deleteSample(const std::string& id);

private:
    SampleRepository& repo;
    std::string       generateId() const;
};
