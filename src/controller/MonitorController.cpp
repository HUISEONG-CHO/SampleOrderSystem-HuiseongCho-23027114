#include "MonitorController.h"

MonitorController::MonitorController(SampleRepository& sampleRepo,
                                     OrderRepository&  orderRepo)
    : sampleRepo(sampleRepo), orderRepo(orderRepo) {}

std::map<OrderStatus, int> MonitorController::getOrderCounts() const {
    std::map<OrderStatus, int> counts;
    for (const auto& o : orderRepo.findAll()) {
        if (o.getStatus() == OrderStatus::REJECTED) continue;
        counts[o.getStatus()]++;
    }
    return counts;
}

std::string MonitorController::getStockLabel(const std::string& sampleId) const {
    Sample sample = sampleRepo.findById(sampleId);
    int stock = sample.getStock();
    if (stock == 0) return "고갈";

    int totalQty = 0;
    for (const auto& o : orderRepo.findAll()) {
        if (o.getSampleId() != sampleId) continue;
        if (o.getStatus() == OrderStatus::REJECTED ||
            o.getStatus() == OrderStatus::RELEASE) continue;
        totalQty += o.getQuantity();
    }

    if (stock > totalQty) return "여유";
    return "부족";
}
