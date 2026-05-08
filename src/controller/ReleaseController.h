#pragma once
#include "repository/SampleRepository.h"
#include "repository/OrderRepository.h"
#include <vector>
#include <string>

class ReleaseController {
public:
    ReleaseController(SampleRepository& sampleRepo,
                      OrderRepository&  orderRepo);

    std::vector<Order> getConfirmedOrders() const;
    void               release(const std::string& orderId);

private:
    SampleRepository& sampleRepo;
    OrderRepository&  orderRepo;
};
