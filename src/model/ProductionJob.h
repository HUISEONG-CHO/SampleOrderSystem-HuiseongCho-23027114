#pragma once
#include <string>

class ProductionJob {
public:
    ProductionJob();
    ProductionJob(const std::string& orderId, const std::string& sampleId,
                  int productionQty, int totalTime,
                  const std::string& enqueuedAt);

    const std::string& getOrderId()      const;
    const std::string& getSampleId()     const;
    int                getProductionQty() const;
    int                getTotalTime()    const;
    const std::string& getEnqueuedAt()  const;

private:
    std::string orderId;
    std::string sampleId;
    int         productionQty;
    int         totalTime;
    std::string enqueuedAt;
};
