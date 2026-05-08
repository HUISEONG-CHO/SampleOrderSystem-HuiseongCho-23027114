#pragma once
#include "repository/SampleRepository.h"
#include "repository/OrderRepository.h"
#include "model/Order.h"
#include <map>
#include <string>

class MonitorController {
public:
    MonitorController(SampleRepository& sampleRepo,
                      OrderRepository&  orderRepo);

    std::map<OrderStatus, int> getOrderCounts() const;
    std::string                getStockLabel(const std::string& sampleId) const;

private:
    SampleRepository& sampleRepo;
    OrderRepository&  orderRepo;
};
