#include "ProductionView.h"
#include <iostream>
#include <algorithm>

void ProductionView::showCurrentJob(const ProductionJob& job, int elapsedMinutes) const {
    int total     = job.getTotalTime();
    int pct       = (total > 0) ? std::min(100, elapsedMinutes * 100 / total) : 100;
    int remaining = std::max(0, total - elapsedMinutes);

    const int barWidth = 20;
    int filled = barWidth * pct / 100;

    std::cout << "=== 현재 생산 중 ===\n";
    std::cout << "  주문ID : " << job.getOrderId() << "\n";
    std::cout << "  시료   : " << job.getSampleId() << "\n";
    std::cout << "  생산량 : " << job.getProductionQty() << "\n";
    std::cout << "  진행   : " << elapsedMinutes << "분 / " << total << "분"
              << "  (" << pct << "%)  잔여 " << remaining << "분\n";
    std::cout << "  [";
    for (int i = 0; i < barWidth; ++i)
        std::cout << (i < filled ? '#' : '-');
    std::cout << "]\n";
    std::cout << "===================\n";
}

void ProductionView::showQueue(const std::vector<ProductionJob>& jobs) const {
    std::cout << "=== 생산 대기 큐 (" << jobs.size() << "건) ===\n";
    int idx = 1;
    for (const auto& job : jobs)
        std::cout << "  " << idx++ << ". 주문ID=" << job.getOrderId()
                  << " | 생산량=" << job.getProductionQty() << "\n";
    if (jobs.empty()) std::cout << "  (대기 없음)\n";
    std::cout << "==================\n";
}
