#include "ProductionView.h"
#include <iostream>

void ProductionView::showCurrentJob(const ProductionJob& job) const {
    std::cout << "=== 현재 생산 중 ===\n";
    std::cout << "  주문ID: " << job.getOrderId()
              << " | 시료: " << job.getSampleId()
              << " | 생산량: " << job.getProductionQty()
              << " | 총시간: " << job.getTotalTime() << "분\n";
}

void ProductionView::showQueue(std::queue<ProductionJob> q) const {
    std::cout << "=== 생산 대기 큐 (" << q.size() << "건) ===\n";
    int idx = 1;
    while (!q.empty()) {
        const auto& job = q.front();
        std::cout << "  " << idx++ << ". 주문ID=" << job.getOrderId()
                  << " | 생산량=" << job.getProductionQty() << "\n";
        q.pop();
    }
    if (idx == 1) std::cout << "  (대기 없음)\n";
    std::cout << "==================\n";
}
