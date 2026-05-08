#pragma once
#include "IRepository.h"
#include "model/Sample.h"
#include <string>
#include <vector>

class SampleRepository : public IRepository<Sample> {
public:
    explicit SampleRepository(const std::string& filePath);

    void           save(const Sample& item)                   override;
    Sample         findById(const std::string& id) const      override;
    std::vector<Sample> findAll() const                       override;
    void           update(const std::string& id, const Sample& item) override;
    void           remove(const std::string& id)              override;

private:
    std::string         filePath;
    std::vector<Sample> samples;

    void loadFromFile();
    void saveToFile() const;
};
