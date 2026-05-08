#pragma once
#include "repository/SampleRepository.h"
#include "repository/OrderRepository.h"
#include "repository/ProductionJobRepository.h"
#include "model/ProductionJob.h"

class ProductionController {
public:
    ProductionController(SampleRepository&        sampleRepo,
                         OrderRepository&         orderRepo,
                         ProductionJobRepository& jobRepo);

    void tick();
    bool hasJobs() const;
    const ProductionJob* currentJob() const;
    int  elapsedMinutes(const ProductionJob& job) const;
    std::vector<ProductionJob> getAllJobs() const;

private:
    SampleRepository&        sampleRepo;
    OrderRepository&         orderRepo;
    ProductionJobRepository& jobRepo;

    std::string currentTimestamp() const;
};
