#include "ReleaseView.h"
#include <iostream>

void ReleaseView::showConfirmedOrders(const std::vector<Order>& orders) const {
    std::cout << "=== 출고 가능 주문 (" << orders.size() << "건) ===\n";
    for (const auto& o : orders) {
        std::cout << "  [" << o.getId() << "] " << o.getCustomer()
                  << " | 시료: " << o.getSampleId()
                  << " | 수량: " << o.getQuantity() << "\n";
    }
    std::cout << "========================\n";
}

void ReleaseView::showReleaseResult(const std::string& orderId) const {
    std::cout << "  주문 [" << orderId << "] 출고 완료\n";
}
