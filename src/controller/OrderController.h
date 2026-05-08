#pragma once
#include "repository/SampleRepository.h"
#include "repository/OrderRepository.h"
#include "model/ProductionJob.h"
#include <string>
#include <queue>

class OrderController {
public:
    OrderController(SampleRepository& sampleRepo,
                    OrderRepository&  orderRepo,
                    std::queue<ProductionJob>& productionQueue);

    std::string placeOrder(const std::string& sampleId,
                           const std::string& customer,
                           int quantity);
    void approveOrder(const std::string& orderId);
    void rejectOrder(const std::string& orderId);

private:
    SampleRepository&         sampleRepo;
    OrderRepository&          orderRepo;
    std::queue<ProductionJob>& productionQueue;

    std::string generateOrderId() const;
    std::string currentTimestamp() const;
};
