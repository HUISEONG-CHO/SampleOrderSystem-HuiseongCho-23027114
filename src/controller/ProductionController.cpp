#include "ProductionController.h"

ProductionController::ProductionController(SampleRepository& sampleRepo,
                                           OrderRepository&  orderRepo,
                                           std::queue<ProductionJob>& jobQueue)
    : sampleRepo(sampleRepo), orderRepo(orderRepo), jobQueue(jobQueue) {}

void ProductionController::completeProduction() {
    if (jobQueue.empty()) return;

    ProductionJob job = jobQueue.front();
    jobQueue.pop();

    Sample sample = sampleRepo.findById(job.getSampleId());
    sample.setStock(sample.getStock() + job.getProductionQty());
    sampleRepo.update(job.getSampleId(), sample);

    Order order = orderRepo.findById(job.getOrderId());
    order.setStatus(OrderStatus::CONFIRMED);
    orderRepo.update(job.getOrderId(), order);
}

bool ProductionController::hasJobs() const {
    return !jobQueue.empty();
}

const ProductionJob* ProductionController::currentJob() const {
    if (jobQueue.empty()) return nullptr;
    return &jobQueue.front();
}
