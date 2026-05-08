#include "OrderView.h"
#include <iostream>

std::string OrderView::statusToStr(OrderStatus s) {
    switch (s) {
        case OrderStatus::RESERVED:  return "접수";
        case OrderStatus::REJECTED:  return "거절";
        case OrderStatus::PRODUCING: return "생산중";
        case OrderStatus::CONFIRMED: return "확정";
        case OrderStatus::RELEASE:   return "출고";
    }
    return "";
}

void OrderView::showOrder(const Order& o) const {
    std::cout << "  [" << o.getId() << "] " << o.getCustomer()
              << " | 시료: " << o.getSampleId()
              << " | 수량: " << o.getQuantity()
              << " | 상태: " << statusToStr(o.getStatus()) << "\n";
}

void OrderView::showAllOrders(const std::vector<Order>& orders) const {
    std::cout << "=== 주문 목록 (" << orders.size() << "건) ===\n";
    for (const auto& o : orders) showOrder(o);
    std::cout << "========================\n";
}
