#include "ProductionView.h"
#include "ui.h"
#include <iostream>
#include <algorithm>

using namespace UI;

void ProductionView::showCurrentJob(const ProductionJob& job, int elapsedMinutes) const {
    int total     = job.getTotalTime();
    int pct       = (total > 0) ? std::min(100, elapsedMinutes * 100 / total) : 100;
    int remaining = std::max(0, total - elapsedMinutes);

    const int barW = 20;
    int filled = barW * pct / 100;

    const char* barColor = (pct >= 80) ? B_GREEN : (pct >= 40 ? CYAN : YELLOW);

    std::string bar;
    for (int i = 0; i < barW; ++i)
        bar += (i < filled ? "█" : "░");

    std::cout << "\n  " << B_CYAN << "생산라인 현황" << RESET << "\n";
    std::cout << "  ┌────────────────────────────────────────────┐\n";
    std::cout << "  │  " << BOLD << "주문 ID   " << RESET << ":  " << padRight(job.getOrderId(), 30) << "│\n";
    std::cout << "  │  " << BOLD << "시료 ID   " << RESET << ":  " << padRight(job.getSampleId(), 30) << "│\n";
    std::cout << "  │  " << BOLD << "생산량    " << RESET << ":  " << padRight(std::to_string(job.getProductionQty()) + "개", 30) << "│\n";
    std::cout << "  ├────────────────────────────────────────────┤\n";
    std::cout << "  │  " << BOLD << "진행률    " << RESET << ":  "
              << barColor << bar << RESET
              << "  " << BOLD << padLeft(std::to_string(pct), 3) << "%" << RESET
              << "  │\n";
    std::cout << "  │  " << BOLD << "경과/총   " << RESET << ":  "
              << CYAN << padLeft(std::to_string(elapsedMinutes), 5) << "분" << RESET
              << " / "
              << padLeft(std::to_string(total), 5) << "분"
              << "   잔여 " << YELLOW << padLeft(std::to_string(remaining), 5) << "분" << RESET
              << "  │\n";
    std::cout << "  └────────────────────────────────────────────┘\n";
}

void ProductionView::showQueue(const std::vector<ProductionJob>& jobs) const {
    std::cout << "\n  " << B_WHITE << "생산 대기 큐 (" << jobs.size() << "건)" << RESET << "\n";
    if (jobs.empty()) {
        std::cout << "  " << DIM << "(대기 중인 작업 없음)" << RESET << "\n";
        return;
    }
    std::cout << "  ┌────┬──────────────┬────────────┐\n";
    std::cout << "  │ "  << BOLD << "No" << RESET
              << " │ " << BOLD << padRight("주문 ID", 12) << RESET
              << " │ " << BOLD << padLeft("생산량", 10) << RESET << " │\n";
    std::cout << "  ├────┼──────────────┼────────────┤\n";
    int idx = 1;
    for (const auto& job : jobs) {
        std::cout << "  │ " << padLeft(std::to_string(idx++), 2)
                  << " │ " << padRight(job.getOrderId(), 12)
                  << " │ " << padLeft(std::to_string(job.getProductionQty()) + "개", 10) << " │\n";
    }
    std::cout << "  └────┴──────────────┴────────────┘\n";
}
