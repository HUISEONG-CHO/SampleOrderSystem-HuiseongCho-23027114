#pragma once
#include "model/Order.h"
#include <vector>

class OrderView {
public:
    void showOrder(const Order& order) const;
    void showAllOrders(const std::vector<Order>& orders) const;

private:
    static std::string statusToStr(OrderStatus s);
};
