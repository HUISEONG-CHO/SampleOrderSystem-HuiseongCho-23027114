#include "SampleView.h"
#include "ui.h"
#include <iostream>
#include <string>

using namespace UI;

// col content widths (visual)
static const int CW_ID    = 3;
static const int CW_NAME  = 14;
static const int CW_TIME  = 8;   // fits "생산시간" (8 visual)
static const int CW_YIELD = 5;
static const int CW_STOCK = 5;

static void sampleTableSep(const char* l, const char* m, const char* r) {
    std::cout << "  " << l
              << rep("─", CW_ID+2)   << m
              << rep("─", CW_NAME+2) << m
              << rep("─", CW_TIME+2) << m
              << rep("─", CW_YIELD+2)<< m
              << rep("─", CW_STOCK+2)<< r << "\n";
}

void SampleView::showSample(const Sample& s) const {
    std::string yield_str = fmtDouble(s.getYieldRate() * 100) + "%";
    std::string stock_str = std::to_string(s.getStock());
    std::string time_str  = std::to_string(s.getAvgProductionTime()) + "분";

    const char* sc = (s.getStock() == 0)    ? B_RED :
                     (s.getStock() < 10)     ? B_YELLOW : B_GREEN;

    std::cout << "  │ "
              << padLeft(s.getId(), CW_ID)         << " │ "
              << padRight(s.getName(), CW_NAME)    << " │ "
              << padLeft(time_str, CW_TIME)        << " │ "
              << padLeft(yield_str, CW_YIELD)      << " │ "
              << sc << padLeft(stock_str, CW_STOCK) << RESET
              << " │\n";
}

void SampleView::showAllSamples(const std::vector<Sample>& samples) const {
    std::cout << "\n  " << B_WHITE << "시료 목록 (" << samples.size() << "개)" << RESET << "\n";
    sampleTableSep("┌", "┬", "┐");
    std::cout << "  │ "
              << BOLD << padRight("ID",    CW_ID)    << RESET << " │ "
              << BOLD << padRight("시료명", CW_NAME)  << RESET << " │ "
              << BOLD << padLeft("생산시간",CW_TIME)  << RESET << " │ "
              << BOLD << padLeft("수율",   CW_YIELD)  << RESET << " │ "
              << BOLD << padLeft("재고",   CW_STOCK)  << RESET << " │\n";
    sampleTableSep("├", "┼", "┤");
    if (samples.empty()) {
        std::cout << "  │ " << DIM
                  << padRight("(등록된 시료 없음)", CW_ID+CW_NAME+CW_TIME+CW_YIELD+CW_STOCK+8)
                  << RESET << " │\n";
    } else {
        for (const auto& s : samples) showSample(s);
    }
    sampleTableSep("└", "┴", "┘");
}
