#include "ProductionJob.h"

ProductionJob::ProductionJob() : productionQty(0), totalTime(0) {}

ProductionJob::ProductionJob(const std::string& orderId, const std::string& sampleId,
                             int productionQty, int totalTime,
                             const std::string& enqueuedAt)
    : orderId(orderId), sampleId(sampleId),
      productionQty(productionQty), totalTime(totalTime), enqueuedAt(enqueuedAt) {}

const std::string& ProductionJob::getOrderId()       const { return orderId; }
const std::string& ProductionJob::getSampleId()      const { return sampleId; }
int                ProductionJob::getProductionQty() const { return productionQty; }
int                ProductionJob::getTotalTime()     const { return totalTime; }
const std::string& ProductionJob::getEnqueuedAt()   const { return enqueuedAt; }
