#include "SampleView.h"
#include <iostream>

void SampleView::showSample(const Sample& s) const {
    std::cout << "  [" << s.getId() << "] " << s.getName()
              << " | 생산시간: " << s.getAvgProductionTime() << "분"
              << " | 수율: " << s.getYieldRate()
              << " | 재고: " << s.getStock() << "\n";
}

void SampleView::showAllSamples(const std::vector<Sample>& samples) const {
    std::cout << "=== 시료 목록 (" << samples.size() << "개) ===\n";
    for (const auto& s : samples) showSample(s);
    std::cout << "========================\n";
}
