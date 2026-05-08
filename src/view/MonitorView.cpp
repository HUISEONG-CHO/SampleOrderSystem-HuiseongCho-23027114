#include "MonitorView.h"
#include <iostream>

void MonitorView::showOrderCounts(const std::map<OrderStatus, int>& counts) const {
    auto get = [&](OrderStatus s) {
        auto it = counts.find(s);
        return it != counts.end() ? it->second : 0;
    };
    std::cout << "=== 주문 현황 ===\n";
    std::cout << "  접수(RESERVED) : " << get(OrderStatus::RESERVED)  << "\n";
    std::cout << "  생산중         : " << get(OrderStatus::PRODUCING) << "\n";
    std::cout << "  확정           : " << get(OrderStatus::CONFIRMED) << "\n";
    std::cout << "  출고           : " << get(OrderStatus::RELEASE)   << "\n";
    std::cout << "================\n";
}

void MonitorView::showStockLabel(const std::string& sampleId,
                                  const std::string& name,
                                  int stock,
                                  const std::string& label) const {
    std::cout << "  [" << sampleId << "] " << name
              << " | 재고: " << stock
              << " | 상태: " << label << "\n";
}
