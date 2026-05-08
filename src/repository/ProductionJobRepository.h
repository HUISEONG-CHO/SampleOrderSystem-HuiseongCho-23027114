#pragma once
#include "IRepository.h"
#include "model/ProductionJob.h"
#include <vector>
#include <string>

class ProductionJobRepository : public IRepository<ProductionJob> {
public:
    explicit ProductionJobRepository(const std::string& filePath);

    void                       save(const ProductionJob& item)                         override;
    ProductionJob              findById(const std::string& orderId)               const override;
    std::vector<ProductionJob> findAll()                                          const override;
    void                       update(const std::string& orderId, const ProductionJob& item) override;
    void                       remove(const std::string& orderId)                      override;

private:
    std::string                filePath;
    std::vector<ProductionJob> jobs;

    void loadFromFile();
    void saveToFile() const;
};
