#include "MonitorView.h"
#include "ui.h"
#include <iostream>
#include <algorithm>

using namespace UI;

static std::string miniBar(int val, int maxVal, int barW = 12) {
    int filled = (maxVal > 0) ? std::min(barW, val * barW / maxVal) : 0;
    std::string bar;
    for (int i = 0; i < barW; ++i)
        bar += (i < filled ? "█" : "░");
    return bar;
}

void MonitorView::showOrderCounts(const std::map<OrderStatus, int>& counts) const {
    auto get = [&](OrderStatus s) {
        auto it = counts.find(s);
        return it != counts.end() ? it->second : 0;
    };
    int res = get(OrderStatus::RESERVED);
    int pro = get(OrderStatus::PRODUCING);
    int con = get(OrderStatus::CONFIRMED);
    int rel = get(OrderStatus::RELEASE);
    int maxVal = std::max({res, pro, con, rel, 1});

    std::cout << "\n  " << B_WHITE << "현황 모니터링" << RESET << "\n";
    std::cout << "  ┌──────────────────────────────────────────┐\n";
    std::cout << "  │  " << BOLD << "주문 현황" << RESET << "                                │\n";
    std::cout << "  ├──────────────────────────────────────────┤\n";

    auto row = [&](const char* color, const char* label, int val, int mx) {
        std::cout << "  │  " << color << padRight(label, 6) << RESET
                  << "  " << color << miniBar(val, mx) << RESET
                  << "  " << BOLD << padLeft(std::to_string(val), 3) << RESET << "건"
                  << "                  │\n";
    };
    row(YELLOW,  "접수중",  res, maxVal);
    row(CYAN,    "생산중",  pro, maxVal);
    row(B_GREEN, "확정",    con, maxVal);
    row(BLUE,    "출고완료",rel, maxVal);

    std::cout << "  └──────────────────────────────────────────┘\n";
}

void MonitorView::showStockLabel(const std::string& sampleId,
                                  const std::string& name,
                                  int stock,
                                  const std::string& label) const {
    const char* sc = (stock == 0)   ? B_RED :
                     (stock < 10)   ? B_YELLOW : B_GREEN;
    std::cout << "  │  " << padLeft(sampleId, 3)   << "  "
              << padRight(name, 14) << "  재고 "
              << sc << padLeft(std::to_string(stock), 5) << RESET
              << "  " << sc << padRight(label, 4) << RESET
              << "  │\n";
}

void MonitorView::showStockHeader() const {
    std::cout << "  ├──────────────────────────────────────────┤\n";
    std::cout << "  │  " << BOLD << "시료별 재고 현황" << RESET
              << "                          │\n";
    std::cout << "  ├──────────────────────────────────────────┤\n";
}

void MonitorView::showStockFooter() const {
    std::cout << "  └──────────────────────────────────────────┘\n";
}
