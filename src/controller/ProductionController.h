#pragma once
#include "repository/SampleRepository.h"
#include "repository/OrderRepository.h"
#include "model/ProductionJob.h"
#include <queue>

class ProductionController {
public:
    ProductionController(SampleRepository& sampleRepo,
                         OrderRepository&  orderRepo,
                         std::queue<ProductionJob>& jobQueue);

    void completeProduction();
    bool hasJobs() const;
    const ProductionJob* currentJob() const;

private:
    SampleRepository&          sampleRepo;
    OrderRepository&           orderRepo;
    std::queue<ProductionJob>& jobQueue;
};
