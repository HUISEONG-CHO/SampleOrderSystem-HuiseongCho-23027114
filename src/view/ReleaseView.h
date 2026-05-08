#pragma once
#include "model/Order.h"
#include <vector>

class ReleaseView {
public:
    void showConfirmedOrders(const std::vector<Order>& orders) const;
    void showReleaseResult(const std::string& orderId) const;
};
