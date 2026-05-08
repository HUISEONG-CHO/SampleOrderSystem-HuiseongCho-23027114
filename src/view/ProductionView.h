#pragma once
#include "model/ProductionJob.h"
#include <vector>

class ProductionView {
public:
    void showCurrentJob(const ProductionJob& job, int elapsedMinutes) const;
    void showQueue(const std::vector<ProductionJob>& jobs) const;
};
