#pragma once
#include "model/ProductionJob.h"
#include <queue>

class ProductionView {
public:
    void showCurrentJob(const ProductionJob& job) const;
    void showQueue(std::queue<ProductionJob> q) const;
};
