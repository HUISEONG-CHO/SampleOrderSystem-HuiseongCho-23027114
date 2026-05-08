#include "ReleaseView.h"
#include "ui.h"
#include <iostream>

using namespace UI;

static const int RW_ID   = 3;
static const int RW_CUST = 12;
static const int RW_SID  = 6;
static const int RW_QTY  = 5;

static void releaseTableSep(const char* l, const char* m, const char* r) {
    std::cout << "  " << l
              << rep("─", RW_ID+2)  << m
              << rep("─", RW_CUST+2)<< m
              << rep("─", RW_SID+2) << m
              << rep("─", RW_QTY+2) << r << "\n";
}

void ReleaseView::showConfirmedOrders(const std::vector<Order>& orders) const {
    std::cout << "\n  " << B_WHITE << "출고 가능 주문 (" << orders.size() << "건)" << RESET << "\n";
    releaseTableSep("┌", "┬", "┐");
    std::cout << "  │ "
              << BOLD << padRight("ID",    RW_ID)   << RESET << " │ "
              << BOLD << padRight("고객명", RW_CUST) << RESET << " │ "
              << BOLD << padLeft("시료 ID", RW_SID)  << RESET << " │ "
              << BOLD << padLeft("수량",   RW_QTY)   << RESET << " │\n";
    releaseTableSep("├", "┼", "┤");
    if (orders.empty()) {
        std::cout << "  │ " << DIM
                  << padRight("(출고 가능한 주문 없음)", RW_ID+RW_CUST+RW_SID+RW_QTY+6)
                  << RESET << " │\n";
    } else {
        for (const auto& o : orders) {
            std::cout << "  │ "
                      << padLeft(o.getId(), RW_ID)               << " │ "
                      << padRight(o.getCustomer(), RW_CUST)      << " │ "
                      << padLeft(o.getSampleId(), RW_SID)        << " │ "
                      << padLeft(std::to_string(o.getQuantity()), RW_QTY) << " │\n";
        }
    }
    releaseTableSep("└", "┴", "┘");
}

void ReleaseView::showReleaseResult(const std::string& orderId) const {
    std::string msg = "  주문 [" + orderId + "] 출고 완료  ";
    size_t w = displayWidth(msg);
    std::cout << "\n  " << B_GREEN << "┌" << rep("─", (int)w) << "┐" << RESET << "\n";
    std::cout << "  " << B_GREEN << "│" << RESET << msg << B_GREEN << "│" << RESET << "\n";
    std::cout << "  " << B_GREEN << "└" << rep("─", (int)w) << "┘" << RESET << "\n";
}
